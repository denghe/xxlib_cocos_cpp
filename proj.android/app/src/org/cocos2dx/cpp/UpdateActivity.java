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
    private TextView tv_msg;
    private MyReceiver receiver;
    private String pack_type;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // 预填充上下文变量
        ApplicationInfo ai = null;
        try {
            ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        bundle = ai.metaData;

        app_tag = bundle.getString("app_tag");
        app_so = bundle.getString("app_so");
        app_check_url = bundle.getString("app_check_url");
        app_receiver_key = bundle.getString("app_receiver_key");

        writable_path = getFilesDir().getAbsolutePath();
        so_path = writable_path + "/lib" + app_so + ".so";
        zip_path = writable_path + "/update.zip";

        so_install_path = "/data/data/" + getPackageName() + "/lib/lib" + app_so + ".so";

        AppActivity.libraryName = app_so;


        // 注册监听器 for 优雅关闭 这个 activity 避免显示进程列表的时候 出现多个
        receiver = new MyReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(app_receiver_key);
        registerReceiver(receiver, filter);

        // 初始化显示
        setContentView(R.layout.activity_main);
        tv_msg = findViewById(R.id.tv_msg);
        tv_msg.setText("check update...");
        pack_type = "test";
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
        new Thread(new Runnable() {
            @Override
            public void run() {

                if (!new File(so_path).exists()) {
                    try {
                        cloneSo2Sdcard(so_install_path, so_path, false);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                String info = getUpdateInfo();
                Log.d(app_tag, info);
                if (info != "success") {
                    //开始更新
                    if (!downloadZip(info)) {
                        // todo: 提示 重试?
                    }
                }

                UpdateActivity.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Intent intent = new Intent(UpdateActivity.this, AppActivity.class);
                        startActivityForResult(intent, 0);

                        Intent closeNotify = new Intent();
                        closeNotify.setAction(app_receiver_key);
                        UpdateActivity.this.sendBroadcast(closeNotify);
                    }
                });
            }
        }).start();
    }


    public String getUpdateInfo() {
        String result = "";

        String md5 = getFileMD5(so_path);
        String url = app_check_url + pack_type + "/" + (is64BitImpl() ? "2" : "1") + "/" + md5;
        Log.d(app_tag, url);

        // 生成请求对象
        HttpGet httpGet = new HttpGet(url);
        HttpClient httpClient = new DefaultHttpClient();
        try {
            HttpResponse response = httpClient.execute(httpGet);
            if (null == response)
                return result;
            HttpEntity httpEntity = response.getEntity();
            try {
                InputStream inputStream = httpEntity.getContent();
                BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
                String line = "";
                while (null != (line = reader.readLine()))
                    result += line;
                Log.i(app_tag, "result = " + result);
            } catch (Exception e) {
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
            return "success";
        }
        return result;
    }

    //查询当前是否是64位系统
    private static boolean is64BitImpl() {
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

    // 下载 更新文件
    private boolean downloadZip(String httpurl) {

        String[] spurl = httpurl.split("\\|");
        if (spurl.length == 3) {
            URL url = null;
            InputStream is = null;
            FileOutputStream fos = null;
            try {
                url = new URL(spurl[2]);
                //开启连接
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setConnectTimeout(30000);
                //设置获取图片的方式为GET
                conn.setRequestMethod("GET");
                //响应码为200，则访问成功
                if (conn.getResponseCode() == 200) {
                    is = conn.getInputStream();
                    File tmpfile = new File(zip_path);
                    if (tmpfile.exists())
                        tmpfile.delete();

                    double filelength = conn.getContentLength();
                    double currentlength = 0;
                    fos = new FileOutputStream(tmpfile);
                    int len = 0;
                    byte[] buffer = new byte[4096];
                    //将输入流写入到我们定义好的文件中
                    while ((len = is.read(buffer)) != -1) {
                        fos.write(buffer, 0, len);
                        currentlength += len;
                        final double p = currentlength / filelength * 100;
                        tv_msg.post(new Runnable() {
                            @Override
                            public void run() {
                                tv_msg.setText("update:" + String.format("%.1f", p) + "%");
                            }
                        });
                    }
                    //将缓冲刷入文件
                    fos.flush();

                    Log.d(app_tag, "down:" + spurl[2] + " close");
                    String md5 = getFileMD5(zip_path);
                    Log.d(app_tag, spurl[0].toLowerCase());
                    Log.d(app_tag, "downzipMD5:" + md5 + " close");

                    if (!spurl[0].toLowerCase().contains(md5.toLowerCase())) {
                        Log.d(app_tag, "zip file md5 check error");
                        return false;
                    }

                    Log.d(app_tag, zip_path);
                    Log.d(app_tag, writable_path);
                    decompressionFile(zip_path, writable_path);

                    String somd5 = getFileMD5(so_path);

                    if (somd5.toLowerCase().contains(spurl[1].toLowerCase())) {
                        Log.d(app_tag, "so update ok");
                        return true;
                    } else {
                        Log.d(app_tag, "so file md5 check error");
                        return false;
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                //在最后，将各种流关闭
                try {
                    if (is != null) {
                        is.close();
                    }
                    if (fos != null) {
                        fos.close();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return false;
            }
        }
        return true;
    }

    // 解压文件
    public static void decompressionFile(String srcPath, String outPath) throws IOException {
        //简单判断解压路径是否合法
        if (!new File(srcPath).isDirectory()) {
            //判断输出路径是否合法
            if (new File(outPath).isDirectory()) {
                if (!outPath.endsWith(File.separator)) {
                    outPath += File.separator;
                }
                //zip读取压缩文件
                FileInputStream fileInputStream = new FileInputStream(srcPath);
                ZipInputStream zipInputStream = new ZipInputStream(fileInputStream);
                //解压文件
                decompressionFile(outPath, zipInputStream);
                //关闭流
                zipInputStream.close();
                fileInputStream.close();
            } else {
                throw new RuntimeException("输出路径不合法!");
            }
        } else {
            throw new RuntimeException("需要解压的文件不合法!");
        }
    }

    // 解压节点
    private static void decompressionFile(String outPath, ZipInputStream inputStream) throws IOException {
        ZipEntry nextEntry = inputStream.getNextEntry();
        while (nextEntry != null) {
            String name = nextEntry.getName();
            File file = new File(outPath + name);
            if (name.endsWith("/")) {
                file.mkdir();
            } else {
                //文件则写入具体的路径中
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

    // 返回MD5
    public static String getFileMD5(String filePath) {
        String md5 = null;
        File file = new File(filePath);
        if (!file.isFile()) {
            return null;
        }
        MessageDigest digest = null;
        FileInputStream in = null;
        byte buffer[] = new byte[1024];
        int len;
        try {
            digest = MessageDigest.getInstance("MD5");
            in = new FileInputStream(file);
            while ((len = in.read(buffer, 0, 1024)) != -1) {
                digest.update(buffer, 0, len);
            }
            in.close();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        BigInteger bigInt = new BigInteger(1, digest.digest());
        md5 = bigInt.toString(16);
        Log.d(app_tag, "md5 = " + md5);
        return md5;
    }

    // 复制文件到指定到目录
    public static boolean cloneSo2Sdcard(String srcFileName, String destFileName, boolean reWrite) throws IOException {
        Log.d(app_tag, "copyFile, begin");
        File srcFile = new File(srcFileName);
        File destFile = new File(destFileName);
        if (!srcFile.exists()) {
            Log.d(app_tag, "copyFile, source file not exist:" + srcFileName);
            return false;
        }
        if (!srcFile.isFile()) {
            Log.d(app_tag, "copyFile, source file not a file:" + srcFileName);
            return false;
        }
        if (!srcFile.canRead()) {
            Log.d(app_tag, "copyFile, source file can't read.");
            return false;
        }
        if (destFile.exists() && reWrite) {
            Log.d(app_tag, "copyFile, before copy File, delete first.");
            destFile.delete();
        }

        try {
            InputStream inStream = new FileInputStream(srcFile);
            FileOutputStream outStream = new FileOutputStream(destFile);
            byte[] buf = new byte[1024];
            int byteRead = 0;
            while ((byteRead = inStream.read(buf)) != -1) {
                outStream.write(buf, 0, byteRead);
            }
            outStream.flush();
            outStream.close();
            inStream.close();
        } catch (IOException e) {
            throw e;
        } catch (Exception e) {
            e.printStackTrace();
        }

        Log.d(app_tag, "copyToFile:" + destFileName + " success");
        return true;
    }


}
