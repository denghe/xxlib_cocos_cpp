package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import net.denghe.cccpp1.R;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.CoreConnectionPNames;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.math.BigInteger;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.MessageDigest;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class UpdateActivity extends Activity {

    // 方便用 bundle.getString 从 AndroidManifest.xml 的 application 的 <meta-data android:name="key" android:value="value" /> 读数据
    public static Bundle bundle = null;

    // bundle config: 日志输出前缀
    public static String app_tag;

    // bundle config: SO 库名：lib xxxx .so 的 xxxx 部分
    public static String app_so;

    // bundle config: 用于更新 so 网址
    public static String app_check_url;

    // bundle config: 用到的监听器 key ( 需要非常唯一 )
    public static String app_receiver_key;


    // 可写目录路径( 尾部没有 / )
    public static String writable_path;

    // 指向可写目录中的 so 文件名
    public static String so_path;

    // 指向安装目录中的 so 文件名
    public static String so_install_path;

    // 指向可写目录中的 update.zip 文件名
    public static String zip_path;


    // 指向提示文本
    TextView tv_msg;

    // 用于优雅关闭当前 activity
    MyReceiver receiver;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 定位到 bundle 以便读取配置
        ApplicationInfo ai = null;
        try {
            ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        assert ai != null;
        bundle = ai.metaData;

        // 开始填充各种 static 上下文变量
        app_tag = bundle.getString("app_tag");
        app_so = bundle.getString("app_so");
        app_check_url = bundle.getString("app_check_url");
        app_receiver_key = bundle.getString("app_receiver_key");

        writable_path = getFilesDir().getAbsolutePath();
        so_path = writable_path + "/lib" + app_so + ".so";
        zip_path = writable_path + "/update.zip";

        so_install_path = "/data/data/" + getPackageName() + "/lib/lib" + app_so + ".so";

        // 透传库名到 cocos 的 activity
        AppActivity.libraryName = app_so;

        // 指向控件
        tv_msg = findViewById(R.id.tv_msg);

        // 注册监听器 for 优雅关闭 这个 activity 避免显示进程列表的时候 出现多个
        receiver = new MyReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(app_receiver_key);
        registerReceiver(receiver, filter);

        // 开始检查更新
        checkUpdate();
    }

    // 下面几个函数都是配合  优雅关闭 这个 activity
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 0 && resultCode == -1) {
            finish();
        }
    }

    @Override
    protected void onDestroy() {
        unregisterReceiver(receiver);
        super.onDestroy();
    }

    private class MyReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            UpdateActivity.this.finish();
        }
    }


    private void checkUpdate() {
        tv_msg.setText("check update...");
        new Thread(new Runnable() {
            @Override
            public void run() {
                do {
                    // 获取升级信息: so md5 | zip md5 | zip url
                    String info = getUpdateInfo(5000, 15000);

                    // 异常判断：网址访问失败
                    if (info == null) {
                        Log.i(app_tag, "info == null");
                        break;                                     // 打开 url 异常, 继续加载游戏
                    }
                    Log.i(app_tag, "info == '" + info + "'");

                    // 检测完毕: 无下载, 加载 apk 内的原始包
                    if (info.equals("")) {
                        try {
                            deleteFile(so_path);                    // 无脑删一次, 确保加载 apk 中的原始 so
                        } catch (Exception ignored) {
                        }
                        break;                                      // 出 do 执行加载 so 操作
                    }

                    // 从 info 解析出数据
                    String[] ss = info.split("\\|");
                    if (ss.length != 3) {
                        Log.i(app_tag, "ss.length != 3");
                        return;                                     // todo: 提示 切割错误, 重新访问 url?
                    }
                    String info_so_md5 = ss[0].toLowerCase();
                    String info_zip_md5 = ss[1].toLowerCase();
                    String info_url = ss[2];

                    // 如果需要的话, 复制 安装目录的 so 到 可写目录
                    if (!new File(so_path).exists()) {
                        int r = copyFile(so_install_path, so_path);
                        if (r != 0 && r != -2) {    // -2 表示调试模式找不到 src
                            Log.i(app_tag, "copyFile fail. r = " + r);
                            return;                                 // todo: 提示 copy 失败
                        }
                    }

                    // 计算 so 的 md5
                    String so_md5 = getFileMD5(so_path);

                    // 兼容 debug 模式因 so_install_path 找不到 进而 so_path 找不到而导致的空返回值
                    if (so_md5 == null) {
                        so_md5 = " ";  // 令 info_so_md5.contains(so_md5) 为假
                    }

                    // 判断 so 是否需要更新. md5 没变化就不更新
                    if (info_so_md5.contains(so_md5)) {
                        Log.i(app_tag, "info_so_md5 == so_md5");
                        break;                                      // 不需要更新, 出 do 执行加载 so 操作
                    }

                    // 删除之前的 zip
                    try {
                        deleteFile(zip_path);
                    }
                    catch (Exception ignored) {
                    }

                    // 开始下载 zip
                    Log.i(app_tag, "begin download zip");
                    if (!downloadZip(info_url)) {
                        Log.i(app_tag, "downloadZip fail. url == " + info_url);
                        return;                                     // todo: 提示 下载错误? 自动重试? 续传?
                    }

                    // 计算 zip 的 md5
                    String zip_md5 = getFileMD5(zip_path);
                    if (!info_zip_md5.contains(zip_md5)) {
                        Log.i(app_tag, "info_zip_md5'" + info_zip_md5 + "' != zip_md5'" + zip_md5 + "'");
                        return;                                     // todo: 重下? 数次后 提示 数据错误?
                    }

                    // 删除之前的 so
                    try {
                        deleteFile(so_path);
                    }
                    catch (Exception ignored) {
                    }

                    // 解压并覆盖 so
                    Log.i(app_tag, "begin decompression zip");
                    int r = decompressionFile(zip_path, writable_path);
                    if (0 != r) {
                        Log.i(app_tag, "decompressionFile fail r = " + r + ". zip_path = " + zip_path + ", writable_path = " + writable_path);
                        return;                                     // todo: 提示解压失败?
                    }

                } while (false);

                // 切到 cocos 的 activity
                Log.i(app_tag, "switch activity to cocos. ");
                UpdateActivity.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        // 加载 cocos 的 activity
                        Intent activity = new Intent(UpdateActivity.this, AppActivity.class);
                        startActivityForResult(activity, 0);

                        // 产生关闭当前 activity 的通知
                        Intent closeNotify = new Intent();
                        closeNotify.setAction(app_receiver_key);
                        UpdateActivity.this.sendBroadcast(closeNotify);
                    }
                });
            }
        }).start();
    }

    // 拼接出类似 http://xxxxxxx/ package name / a32 这样的 url 去打开并返回内容. 内容为 null 表示访问出错. "" 表示加载原始 so. 有更新: so_md5|zip_md5|zip_url
    public String getUpdateInfo(int connTimeoutMS, int recvTimeoutMS) {
        StringBuilder result = new StringBuilder();
        try {
            String url = app_check_url + getPackageName() + "/" + (isArch64() ? "a64" : "a32");
            HttpGet httpGet = new HttpGet(url);
            HttpClient httpClient = new DefaultHttpClient();
            httpClient.getParams().setParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, connTimeoutMS);
            httpClient.getParams().setParameter(CoreConnectionPNames.SO_TIMEOUT, recvTimeoutMS);
            HttpResponse response = httpClient.execute(httpGet);
            if (null == response) return result.toString();
            HttpEntity httpEntity = response.getEntity();
            InputStream inputStream = httpEntity.getContent();
            BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
            String line;
            while (null != (line = reader.readLine())) {
                result.append(line);
            }
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        return result.toString();
    }

    // 下载 更新文件
    private boolean downloadZip(String urlStr) {
        InputStream is = null;
        FileOutputStream fos = null;
        try {
            URL url = new URL(urlStr);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setConnectTimeout(30000);
            conn.setRequestMethod("GET");
            // 响应码为 200，则访问成功
            if (conn.getResponseCode() == 200) {
                is = conn.getInputStream();
                File tmp = new File(zip_path);
                if (tmp.exists()) {
                    if (!tmp.delete()) return false;
                }
                double fileLength = conn.getContentLength();
                double currentLength = 0;
                fos = new FileOutputStream(tmp);
                int len;
                byte[] buffer = new byte[4096];
                while ((len = is.read(buffer)) != -1) {
                    // 将输入流写入到我们定义好的文件中
                    fos.write(buffer, 0, len);
                    currentLength += len;
                    final double p = currentLength / fileLength * 100;
                    tv_msg.post(new Runnable() {
                        @Override
                        public void run() {
                            tv_msg.setText("update:" + String.format("%.1f", p) + "%");     // 更新下载进度显示
                        }
                    });
                }
                fos.flush();
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (is != null) {
                    is.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                if (fos != null) {
                    fos.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return false;
    }

    // 解压文件
    public static int decompressionFile(String srcPath, String outPath) {
        if (!new File(srcPath).isFile()) return -1;
        if (!new File(outPath).isDirectory()) return -2;
        if (!outPath.endsWith(File.separator)) {
            outPath += File.separator;
        }
        FileInputStream fileInputStream = null;
        ZipInputStream zipInputStream = null;
        try {
            fileInputStream = new FileInputStream(srcPath);
            zipInputStream = new ZipInputStream(fileInputStream);
            decompressionFile(outPath, zipInputStream);
            zipInputStream.close();
            fileInputStream.close();
            return 0;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {

                if (fileInputStream != null) {
                    fileInputStream.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                if (zipInputStream != null) {
                    zipInputStream.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return -3;
    }

    // 解压节点
    private static void decompressionFile(String outPath, ZipInputStream inputStream) throws IOException {
        ZipEntry nextEntry = inputStream.getNextEntry();
        while (nextEntry != null) {
            String name = nextEntry.getName();
            File file = new File(outPath + name);
            if (name.endsWith("/")) {
                if (!file.mkdir()) throw new IOException("create dir fail. dir = " + file);
            } else {
                FileOutputStream fileOutputStream = new FileOutputStream(file);
                BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);
                int n;
                byte[] bytes = new byte[1024];
                while ((n = inputStream.read(bytes)) != -1) {
                    bufferedOutputStream.write(bytes, 0, n);
                }
                bufferedOutputStream.close();
                fileOutputStream.close();
            }
            inputStream.closeEntry();
            nextEntry = inputStream.getNextEntry();
        }
    }

    // 返回 md5 的 16 进制( 小写 ). 出错返回 null
    public static String getFileMD5(String filePath) {
        File file = new File(filePath);
        if (!file.isFile()) {
            return null;
        }
        FileInputStream in = null;
        byte[] buffer = new byte[1024];
        int len;
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            in = new FileInputStream(file);
            while ((len = in.read(buffer, 0, 1024)) != -1) {
                md.update(buffer, 0, len);
            }
            in.close();
            return new BigInteger(1, md.digest()).toString(16);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }


    // 复制文件. 两个参数都是 完整路径的文件名. 如果目标文件已存在则会 删除再复制
    public static int copyFile(String src, String dst) {
        File srcFile = new File(src);
        if (!srcFile.exists()) {
            return -2;                      // source file not exist
        }
        if (!srcFile.isFile()) {
            return -3;                      // source file not a file
        }
        if (!srcFile.canRead()) {
            return -4;                      // source file can't read
        }
        try {
            File destFile = new File(dst);
            if (destFile.exists()) {
                if (!destFile.delete()) return -5;   // dest file delete failed.
            }
            InputStream inStream = new FileInputStream(srcFile);
            FileOutputStream outStream = new FileOutputStream(destFile);
            byte[] buf = new byte[64 * 1024];
            int byteRead;
            while ((byteRead = inStream.read(buf)) != -1) {
                outStream.write(buf, 0, byteRead);
            }
            outStream.flush();
            outStream.close();
            inStream.close();
        } catch (Exception e) {
            e.printStackTrace();
            return -1;                      // io error
        }
        return 0;                           // success
    }

    // 查询当前是否是64位执行模式
    private static boolean isArch64() {
        try {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                // Android API 21之前不支持64位CPU
                return false;
            }

            Class<?> clzVMRuntime = Class.forName("dalvik.system.VMRuntime");
            if (clzVMRuntime == null) {
                return false;
            }
            Method mthVMRuntimeGet = clzVMRuntime.getDeclaredMethod("getRuntime");
            if (mthVMRuntimeGet == null) {
                return false;
            }
            Object objVMRuntime = mthVMRuntimeGet.invoke(null);
            if (objVMRuntime == null) {
                return false;
            }
            Method sVMRuntimeIs64BitMethod = clzVMRuntime.getDeclaredMethod("is64Bit");
            if (sVMRuntimeIs64BitMethod == null) {
                return false;
            }
            Object objIs64Bit = sVMRuntimeIs64BitMethod.invoke(objVMRuntime);
            if (objIs64Bit instanceof Boolean) {
                return (boolean) objIs64Bit;
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
        return false;
    }
}
