#include "FileExts_class_lite.h"
#include "FileExts_class_lite.cpp.inc"
namespace FileExts {
	void PkgGenTypes::RegisterTo(xx::ObjectHelper& oh) {
	}
}

namespace xx {
    void CloneFuncs<FileExts::LockPoint>::Clone1(xx::ObjectHelper &oh, FileExts::LockPoint const& in, FileExts::LockPoint &out) {
        CloneFuncs<float>::Clone1(oh, in.x, out.x);
        CloneFuncs<float>::Clone1(oh, in.y, out.y);
    }
    void CloneFuncs<FileExts::LockPoint>::Clone2(xx::ObjectHelper &oh, FileExts::LockPoint const& in, FileExts::LockPoint &out) {
        CloneFuncs<float>::Clone2(oh, in.x, out.x);
        CloneFuncs<float>::Clone2(oh, in.y, out.y);
    }
	void DataFuncsEx<FileExts::LockPoint, void>::Write(DataWriterEx& dw, FileExts::LockPoint const& in) {
        dw.Write(in.x);
        dw.Write(in.y);
    }
	int DataFuncsEx<FileExts::LockPoint, void>::Read(DataReaderEx& d, FileExts::LockPoint& out) {
        if (int r = d.Read(out.x)) return r;
        if (int r = d.Read(out.y)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::LockPoint, void>::Append(ObjectHelper &oh, FileExts::LockPoint const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::LockPoint, void>::AppendCore(ObjectHelper &oh, FileExts::LockPoint const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"x\":", in.x); 
        xx::AppendEx(oh, ",\"y\":", in.y);
    }
    void CloneFuncs<FileExts::CDCircle>::Clone1(xx::ObjectHelper &oh, FileExts::CDCircle const& in, FileExts::CDCircle &out) {
        CloneFuncs<float>::Clone1(oh, in.x, out.x);
        CloneFuncs<float>::Clone1(oh, in.y, out.y);
        CloneFuncs<float>::Clone1(oh, in.r, out.r);
    }
    void CloneFuncs<FileExts::CDCircle>::Clone2(xx::ObjectHelper &oh, FileExts::CDCircle const& in, FileExts::CDCircle &out) {
        CloneFuncs<float>::Clone2(oh, in.x, out.x);
        CloneFuncs<float>::Clone2(oh, in.y, out.y);
        CloneFuncs<float>::Clone2(oh, in.r, out.r);
    }
	void DataFuncsEx<FileExts::CDCircle, void>::Write(DataWriterEx& dw, FileExts::CDCircle const& in) {
        dw.Write(in.x);
        dw.Write(in.y);
        dw.Write(in.r);
    }
	int DataFuncsEx<FileExts::CDCircle, void>::Read(DataReaderEx& d, FileExts::CDCircle& out) {
        if (int r = d.Read(out.x)) return r;
        if (int r = d.Read(out.y)) return r;
        if (int r = d.Read(out.r)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::CDCircle, void>::Append(ObjectHelper &oh, FileExts::CDCircle const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::CDCircle, void>::AppendCore(ObjectHelper &oh, FileExts::CDCircle const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"x\":", in.x); 
        xx::AppendEx(oh, ",\"y\":", in.y);
        xx::AppendEx(oh, ",\"r\":", in.r);
    }
    void CloneFuncs<FileExts::TimePoint_LockPoints>::Clone1(xx::ObjectHelper &oh, FileExts::TimePoint_LockPoints const& in, FileExts::TimePoint_LockPoints &out) {
        CloneFuncs<float>::Clone1(oh, in.time, out.time);
        CloneFuncs<FileExts::LockPoint>::Clone1(oh, in.mainLockPoint, out.mainLockPoint);
        CloneFuncs<std::vector<FileExts::LockPoint>>::Clone1(oh, in.lockPoints, out.lockPoints);
    }
    void CloneFuncs<FileExts::TimePoint_LockPoints>::Clone2(xx::ObjectHelper &oh, FileExts::TimePoint_LockPoints const& in, FileExts::TimePoint_LockPoints &out) {
        CloneFuncs<float>::Clone2(oh, in.time, out.time);
        CloneFuncs<FileExts::LockPoint>::Clone2(oh, in.mainLockPoint, out.mainLockPoint);
        CloneFuncs<std::vector<FileExts::LockPoint>>::Clone2(oh, in.lockPoints, out.lockPoints);
    }
	void DataFuncsEx<FileExts::TimePoint_LockPoints, void>::Write(DataWriterEx& dw, FileExts::TimePoint_LockPoints const& in) {
        dw.Write(in.time);
        dw.Write(in.mainLockPoint);
        dw.Write(in.lockPoints);
    }
	int DataFuncsEx<FileExts::TimePoint_LockPoints, void>::Read(DataReaderEx& d, FileExts::TimePoint_LockPoints& out) {
        if (int r = d.Read(out.time)) return r;
        if (int r = d.Read(out.mainLockPoint)) return r;
        if (int r = d.Read(out.lockPoints)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::TimePoint_LockPoints, void>::Append(ObjectHelper &oh, FileExts::TimePoint_LockPoints const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::TimePoint_LockPoints, void>::AppendCore(ObjectHelper &oh, FileExts::TimePoint_LockPoints const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"time\":", in.time); 
        xx::AppendEx(oh, ",\"mainLockPoint\":", in.mainLockPoint);
        xx::AppendEx(oh, ",\"lockPoints\":", in.lockPoints);
    }
    void CloneFuncs<FileExts::TimePoint_CDCircles>::Clone1(xx::ObjectHelper &oh, FileExts::TimePoint_CDCircles const& in, FileExts::TimePoint_CDCircles &out) {
        CloneFuncs<float>::Clone1(oh, in.time, out.time);
        CloneFuncs<FileExts::CDCircle>::Clone1(oh, in.maxCDCircle, out.maxCDCircle);
        CloneFuncs<std::vector<FileExts::CDCircle>>::Clone1(oh, in.cdCircles, out.cdCircles);
    }
    void CloneFuncs<FileExts::TimePoint_CDCircles>::Clone2(xx::ObjectHelper &oh, FileExts::TimePoint_CDCircles const& in, FileExts::TimePoint_CDCircles &out) {
        CloneFuncs<float>::Clone2(oh, in.time, out.time);
        CloneFuncs<FileExts::CDCircle>::Clone2(oh, in.maxCDCircle, out.maxCDCircle);
        CloneFuncs<std::vector<FileExts::CDCircle>>::Clone2(oh, in.cdCircles, out.cdCircles);
    }
	void DataFuncsEx<FileExts::TimePoint_CDCircles, void>::Write(DataWriterEx& dw, FileExts::TimePoint_CDCircles const& in) {
        dw.Write(in.time);
        dw.Write(in.maxCDCircle);
        dw.Write(in.cdCircles);
    }
	int DataFuncsEx<FileExts::TimePoint_CDCircles, void>::Read(DataReaderEx& d, FileExts::TimePoint_CDCircles& out) {
        if (int r = d.Read(out.time)) return r;
        if (int r = d.Read(out.maxCDCircle)) return r;
        if (int r = d.Read(out.cdCircles)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::TimePoint_CDCircles, void>::Append(ObjectHelper &oh, FileExts::TimePoint_CDCircles const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::TimePoint_CDCircles, void>::AppendCore(ObjectHelper &oh, FileExts::TimePoint_CDCircles const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"time\":", in.time); 
        xx::AppendEx(oh, ",\"maxCDCircle\":", in.maxCDCircle);
        xx::AppendEx(oh, ",\"cdCircles\":", in.cdCircles);
    }
    void CloneFuncs<FileExts::TimePoint_Speed>::Clone1(xx::ObjectHelper &oh, FileExts::TimePoint_Speed const& in, FileExts::TimePoint_Speed &out) {
        CloneFuncs<float>::Clone1(oh, in.time, out.time);
        CloneFuncs<float>::Clone1(oh, in.speed, out.speed);
    }
    void CloneFuncs<FileExts::TimePoint_Speed>::Clone2(xx::ObjectHelper &oh, FileExts::TimePoint_Speed const& in, FileExts::TimePoint_Speed &out) {
        CloneFuncs<float>::Clone2(oh, in.time, out.time);
        CloneFuncs<float>::Clone2(oh, in.speed, out.speed);
    }
	void DataFuncsEx<FileExts::TimePoint_Speed, void>::Write(DataWriterEx& dw, FileExts::TimePoint_Speed const& in) {
        dw.Write(in.time);
        dw.Write(in.speed);
    }
	int DataFuncsEx<FileExts::TimePoint_Speed, void>::Read(DataReaderEx& d, FileExts::TimePoint_Speed& out) {
        if (int r = d.Read(out.time)) return r;
        if (int r = d.Read(out.speed)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::TimePoint_Speed, void>::Append(ObjectHelper &oh, FileExts::TimePoint_Speed const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::TimePoint_Speed, void>::AppendCore(ObjectHelper &oh, FileExts::TimePoint_Speed const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"time\":", in.time); 
        xx::AppendEx(oh, ",\"speed\":", in.speed);
    }
    void CloneFuncs<FileExts::TimePoint_Frame>::Clone1(xx::ObjectHelper &oh, FileExts::TimePoint_Frame const& in, FileExts::TimePoint_Frame &out) {
        CloneFuncs<float>::Clone1(oh, in.time, out.time);
        CloneFuncs<std::string>::Clone1(oh, in.picName, out.picName);
    }
    void CloneFuncs<FileExts::TimePoint_Frame>::Clone2(xx::ObjectHelper &oh, FileExts::TimePoint_Frame const& in, FileExts::TimePoint_Frame &out) {
        CloneFuncs<float>::Clone2(oh, in.time, out.time);
        CloneFuncs<std::string>::Clone2(oh, in.picName, out.picName);
    }
	void DataFuncsEx<FileExts::TimePoint_Frame, void>::Write(DataWriterEx& dw, FileExts::TimePoint_Frame const& in) {
        dw.Write(in.time);
        dw.Write(in.picName);
    }
	int DataFuncsEx<FileExts::TimePoint_Frame, void>::Read(DataReaderEx& d, FileExts::TimePoint_Frame& out) {
        if (int r = d.Read(out.time)) return r;
        if (int r = d.Read(out.picName)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::TimePoint_Frame, void>::Append(ObjectHelper &oh, FileExts::TimePoint_Frame const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::TimePoint_Frame, void>::AppendCore(ObjectHelper &oh, FileExts::TimePoint_Frame const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"time\":", in.time); 
        xx::AppendEx(oh, ",\"picName\":", in.picName);
    }
    void CloneFuncs<FileExts::Action_Frames>::Clone1(xx::ObjectHelper &oh, FileExts::Action_Frames const& in, FileExts::Action_Frames &out) {
        CloneFuncs<std::string>::Clone1(oh, in.name, out.name);
        CloneFuncs<float>::Clone1(oh, in.totalSeconds, out.totalSeconds);
        CloneFuncs<std::vector<FileExts::TimePoint_Frame>>::Clone1(oh, in.frames, out.frames);
    }
    void CloneFuncs<FileExts::Action_Frames>::Clone2(xx::ObjectHelper &oh, FileExts::Action_Frames const& in, FileExts::Action_Frames &out) {
        CloneFuncs<std::string>::Clone2(oh, in.name, out.name);
        CloneFuncs<float>::Clone2(oh, in.totalSeconds, out.totalSeconds);
        CloneFuncs<std::vector<FileExts::TimePoint_Frame>>::Clone2(oh, in.frames, out.frames);
    }
	void DataFuncsEx<FileExts::Action_Frames, void>::Write(DataWriterEx& dw, FileExts::Action_Frames const& in) {
        dw.Write(in.name);
        dw.Write(in.totalSeconds);
        dw.Write(in.frames);
    }
	int DataFuncsEx<FileExts::Action_Frames, void>::Read(DataReaderEx& d, FileExts::Action_Frames& out) {
        if (int r = d.Read(out.name)) return r;
        if (int r = d.Read(out.totalSeconds)) return r;
        if (int r = d.Read(out.frames)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::Action_Frames, void>::Append(ObjectHelper &oh, FileExts::Action_Frames const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::Action_Frames, void>::AppendCore(ObjectHelper &oh, FileExts::Action_Frames const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"name\":", in.name); 
        xx::AppendEx(oh, ",\"totalSeconds\":", in.totalSeconds);
        xx::AppendEx(oh, ",\"frames\":", in.frames);
    }
    void CloneFuncs<FileExts::Action_AnimExt>::Clone1(xx::ObjectHelper &oh, FileExts::Action_AnimExt const& in, FileExts::Action_AnimExt &out) {
        CloneFuncs<std::string>::Clone1(oh, in.name, out.name);
        CloneFuncs<float>::Clone1(oh, in.totalSeconds, out.totalSeconds);
        CloneFuncs<std::vector<FileExts::TimePoint_LockPoints>>::Clone1(oh, in.lps, out.lps);
        CloneFuncs<std::vector<FileExts::TimePoint_CDCircles>>::Clone1(oh, in.cds, out.cds);
        CloneFuncs<std::vector<FileExts::TimePoint_Speed>>::Clone1(oh, in.ss, out.ss);
    }
    void CloneFuncs<FileExts::Action_AnimExt>::Clone2(xx::ObjectHelper &oh, FileExts::Action_AnimExt const& in, FileExts::Action_AnimExt &out) {
        CloneFuncs<std::string>::Clone2(oh, in.name, out.name);
        CloneFuncs<float>::Clone2(oh, in.totalSeconds, out.totalSeconds);
        CloneFuncs<std::vector<FileExts::TimePoint_LockPoints>>::Clone2(oh, in.lps, out.lps);
        CloneFuncs<std::vector<FileExts::TimePoint_CDCircles>>::Clone2(oh, in.cds, out.cds);
        CloneFuncs<std::vector<FileExts::TimePoint_Speed>>::Clone2(oh, in.ss, out.ss);
    }
	void DataFuncsEx<FileExts::Action_AnimExt, void>::Write(DataWriterEx& dw, FileExts::Action_AnimExt const& in) {
        dw.Write(in.name);
        dw.Write(in.totalSeconds);
        dw.Write(in.lps);
        dw.Write(in.cds);
        dw.Write(in.ss);
    }
	int DataFuncsEx<FileExts::Action_AnimExt, void>::Read(DataReaderEx& d, FileExts::Action_AnimExt& out) {
        if (int r = d.Read(out.name)) return r;
        if (int r = d.Read(out.totalSeconds)) return r;
        if (int r = d.Read(out.lps)) return r;
        if (int r = d.Read(out.cds)) return r;
        if (int r = d.Read(out.ss)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::Action_AnimExt, void>::Append(ObjectHelper &oh, FileExts::Action_AnimExt const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::Action_AnimExt, void>::AppendCore(ObjectHelper &oh, FileExts::Action_AnimExt const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"name\":", in.name); 
        xx::AppendEx(oh, ",\"totalSeconds\":", in.totalSeconds);
        xx::AppendEx(oh, ",\"lps\":", in.lps);
        xx::AppendEx(oh, ",\"cds\":", in.cds);
        xx::AppendEx(oh, ",\"ss\":", in.ss);
    }
    void CloneFuncs<FileExts::PathwayPoint>::Clone1(xx::ObjectHelper &oh, FileExts::PathwayPoint const& in, FileExts::PathwayPoint &out) {
        CloneFuncs<float>::Clone1(oh, in.x, out.x);
        CloneFuncs<float>::Clone1(oh, in.y, out.y);
        CloneFuncs<float>::Clone1(oh, in.tension, out.tension);
        CloneFuncs<int32_t>::Clone1(oh, in.numSegments, out.numSegments);
    }
    void CloneFuncs<FileExts::PathwayPoint>::Clone2(xx::ObjectHelper &oh, FileExts::PathwayPoint const& in, FileExts::PathwayPoint &out) {
        CloneFuncs<float>::Clone2(oh, in.x, out.x);
        CloneFuncs<float>::Clone2(oh, in.y, out.y);
        CloneFuncs<float>::Clone2(oh, in.tension, out.tension);
        CloneFuncs<int32_t>::Clone2(oh, in.numSegments, out.numSegments);
    }
	void DataFuncsEx<FileExts::PathwayPoint, void>::Write(DataWriterEx& dw, FileExts::PathwayPoint const& in) {
        dw.Write(in.x);
        dw.Write(in.y);
        dw.Write(in.tension);
        dw.Write(in.numSegments);
    }
	int DataFuncsEx<FileExts::PathwayPoint, void>::Read(DataReaderEx& d, FileExts::PathwayPoint& out) {
        if (int r = d.Read(out.x)) return r;
        if (int r = d.Read(out.y)) return r;
        if (int r = d.Read(out.tension)) return r;
        if (int r = d.Read(out.numSegments)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::PathwayPoint, void>::Append(ObjectHelper &oh, FileExts::PathwayPoint const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::PathwayPoint, void>::AppendCore(ObjectHelper &oh, FileExts::PathwayPoint const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"x\":", in.x); 
        xx::AppendEx(oh, ",\"y\":", in.y);
        xx::AppendEx(oh, ",\"tension\":", in.tension);
        xx::AppendEx(oh, ",\"numSegments\":", in.numSegments);
    }
    void CloneFuncs<FileExts::File_Frames>::Clone1(xx::ObjectHelper &oh, FileExts::File_Frames const& in, FileExts::File_Frames &out) {
        CloneFuncs<std::vector<FileExts::Action_Frames>>::Clone1(oh, in.actions, out.actions);
        CloneFuncs<std::vector<std::string>>::Clone1(oh, in.plists, out.plists);
    }
    void CloneFuncs<FileExts::File_Frames>::Clone2(xx::ObjectHelper &oh, FileExts::File_Frames const& in, FileExts::File_Frames &out) {
        CloneFuncs<std::vector<FileExts::Action_Frames>>::Clone2(oh, in.actions, out.actions);
        CloneFuncs<std::vector<std::string>>::Clone2(oh, in.plists, out.plists);
    }
	void DataFuncsEx<FileExts::File_Frames, void>::Write(DataWriterEx& dw, FileExts::File_Frames const& in) {
        dw.Write(in.actions);
        dw.Write(in.plists);
    }
	int DataFuncsEx<FileExts::File_Frames, void>::Read(DataReaderEx& d, FileExts::File_Frames& out) {
        if (int r = d.Read(out.actions)) return r;
        if (int r = d.Read(out.plists)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::File_Frames, void>::Append(ObjectHelper &oh, FileExts::File_Frames const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::File_Frames, void>::AppendCore(ObjectHelper &oh, FileExts::File_Frames const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"actions\":", in.actions); 
        xx::AppendEx(oh, ",\"plists\":", in.plists);
    }
    void CloneFuncs<FileExts::File_AnimExt>::Clone1(xx::ObjectHelper &oh, FileExts::File_AnimExt const& in, FileExts::File_AnimExt &out) {
        CloneFuncs<std::vector<FileExts::Action_AnimExt>>::Clone1(oh, in.actions, out.actions);
        CloneFuncs<float>::Clone1(oh, in.shadowX, out.shadowX);
        CloneFuncs<float>::Clone1(oh, in.shadowY, out.shadowY);
        CloneFuncs<float>::Clone1(oh, in.shadowScale, out.shadowScale);
    }
    void CloneFuncs<FileExts::File_AnimExt>::Clone2(xx::ObjectHelper &oh, FileExts::File_AnimExt const& in, FileExts::File_AnimExt &out) {
        CloneFuncs<std::vector<FileExts::Action_AnimExt>>::Clone2(oh, in.actions, out.actions);
        CloneFuncs<float>::Clone2(oh, in.shadowX, out.shadowX);
        CloneFuncs<float>::Clone2(oh, in.shadowY, out.shadowY);
        CloneFuncs<float>::Clone2(oh, in.shadowScale, out.shadowScale);
    }
	void DataFuncsEx<FileExts::File_AnimExt, void>::Write(DataWriterEx& dw, FileExts::File_AnimExt const& in) {
        dw.Write(in.actions);
        dw.Write(in.shadowX);
        dw.Write(in.shadowY);
        dw.Write(in.shadowScale);
    }
	int DataFuncsEx<FileExts::File_AnimExt, void>::Read(DataReaderEx& d, FileExts::File_AnimExt& out) {
        if (int r = d.Read(out.actions)) return r;
        if (int r = d.Read(out.shadowX)) return r;
        if (int r = d.Read(out.shadowY)) return r;
        if (int r = d.Read(out.shadowScale)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::File_AnimExt, void>::Append(ObjectHelper &oh, FileExts::File_AnimExt const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::File_AnimExt, void>::AppendCore(ObjectHelper &oh, FileExts::File_AnimExt const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"actions\":", in.actions); 
        xx::AppendEx(oh, ",\"shadowX\":", in.shadowX);
        xx::AppendEx(oh, ",\"shadowY\":", in.shadowY);
        xx::AppendEx(oh, ",\"shadowScale\":", in.shadowScale);
    }
    void CloneFuncs<FileExts::File_pathway>::Clone1(xx::ObjectHelper &oh, FileExts::File_pathway const& in, FileExts::File_pathway &out) {
        CloneFuncs<bool>::Clone1(oh, in.isLoop, out.isLoop);
        CloneFuncs<std::vector<FileExts::PathwayPoint>>::Clone1(oh, in.points, out.points);
    }
    void CloneFuncs<FileExts::File_pathway>::Clone2(xx::ObjectHelper &oh, FileExts::File_pathway const& in, FileExts::File_pathway &out) {
        CloneFuncs<bool>::Clone2(oh, in.isLoop, out.isLoop);
        CloneFuncs<std::vector<FileExts::PathwayPoint>>::Clone2(oh, in.points, out.points);
    }
	void DataFuncsEx<FileExts::File_pathway, void>::Write(DataWriterEx& dw, FileExts::File_pathway const& in) {
        dw.Write(in.isLoop);
        dw.Write(in.points);
    }
	int DataFuncsEx<FileExts::File_pathway, void>::Read(DataReaderEx& d, FileExts::File_pathway& out) {
        if (int r = d.Read(out.isLoop)) return r;
        if (int r = d.Read(out.points)) return r;
        return 0;
    }
	void StringFuncsEx<FileExts::File_pathway, void>::Append(ObjectHelper &oh, FileExts::File_pathway const& in) {
        oh.s.push_back('{');
        AppendCore(oh, in);
        oh.s.push_back('}');
    }
	void StringFuncsEx<FileExts::File_pathway, void>::AppendCore(ObjectHelper &oh, FileExts::File_pathway const& in) {
        auto sizeBak = oh.s.size();
        if (sizeBak == oh.s.size()) {
            oh.s.push_back(',');
        }
        xx::AppendEx(oh, "\"isLoop\":", in.isLoop); 
        xx::AppendEx(oh, ",\"points\":", in.points);
    }
}
namespace FileExts {
    LockPoint::LockPoint(LockPoint&& o) noexcept {
        this->operator=(std::move(o));
    }
    LockPoint& LockPoint::operator=(LockPoint&& o) noexcept {
        std::swap(this->x, o.x);
        std::swap(this->y, o.y);
        return *this;
    }
    CDCircle::CDCircle(CDCircle&& o) noexcept {
        this->operator=(std::move(o));
    }
    CDCircle& CDCircle::operator=(CDCircle&& o) noexcept {
        std::swap(this->x, o.x);
        std::swap(this->y, o.y);
        std::swap(this->r, o.r);
        return *this;
    }
    TimePoint_LockPoints::TimePoint_LockPoints(TimePoint_LockPoints&& o) noexcept {
        this->operator=(std::move(o));
    }
    TimePoint_LockPoints& TimePoint_LockPoints::operator=(TimePoint_LockPoints&& o) noexcept {
        std::swap(this->time, o.time);
        std::swap(this->mainLockPoint, o.mainLockPoint);
        std::swap(this->lockPoints, o.lockPoints);
        return *this;
    }
    TimePoint_CDCircles::TimePoint_CDCircles(TimePoint_CDCircles&& o) noexcept {
        this->operator=(std::move(o));
    }
    TimePoint_CDCircles& TimePoint_CDCircles::operator=(TimePoint_CDCircles&& o) noexcept {
        std::swap(this->time, o.time);
        std::swap(this->maxCDCircle, o.maxCDCircle);
        std::swap(this->cdCircles, o.cdCircles);
        return *this;
    }
    TimePoint_Speed::TimePoint_Speed(TimePoint_Speed&& o) noexcept {
        this->operator=(std::move(o));
    }
    TimePoint_Speed& TimePoint_Speed::operator=(TimePoint_Speed&& o) noexcept {
        std::swap(this->time, o.time);
        std::swap(this->speed, o.speed);
        return *this;
    }
    TimePoint_Frame::TimePoint_Frame(TimePoint_Frame&& o) noexcept {
        this->operator=(std::move(o));
    }
    TimePoint_Frame& TimePoint_Frame::operator=(TimePoint_Frame&& o) noexcept {
        std::swap(this->time, o.time);
        std::swap(this->picName, o.picName);
        return *this;
    }
    Action_Frames::Action_Frames(Action_Frames&& o) noexcept {
        this->operator=(std::move(o));
    }
    Action_Frames& Action_Frames::operator=(Action_Frames&& o) noexcept {
        std::swap(this->name, o.name);
        std::swap(this->totalSeconds, o.totalSeconds);
        std::swap(this->frames, o.frames);
        return *this;
    }
    Action_AnimExt::Action_AnimExt(Action_AnimExt&& o) noexcept {
        this->operator=(std::move(o));
    }
    Action_AnimExt& Action_AnimExt::operator=(Action_AnimExt&& o) noexcept {
        std::swap(this->name, o.name);
        std::swap(this->totalSeconds, o.totalSeconds);
        std::swap(this->lps, o.lps);
        std::swap(this->cds, o.cds);
        std::swap(this->ss, o.ss);
        return *this;
    }
    PathwayPoint::PathwayPoint(PathwayPoint&& o) noexcept {
        this->operator=(std::move(o));
    }
    PathwayPoint& PathwayPoint::operator=(PathwayPoint&& o) noexcept {
        std::swap(this->x, o.x);
        std::swap(this->y, o.y);
        std::swap(this->tension, o.tension);
        std::swap(this->numSegments, o.numSegments);
        return *this;
    }
    File_Frames::File_Frames(File_Frames&& o) noexcept {
        this->operator=(std::move(o));
    }
    File_Frames& File_Frames::operator=(File_Frames&& o) noexcept {
        std::swap(this->actions, o.actions);
        std::swap(this->plists, o.plists);
        return *this;
    }
    File_AnimExt::File_AnimExt(File_AnimExt&& o) noexcept {
        this->operator=(std::move(o));
    }
    File_AnimExt& File_AnimExt::operator=(File_AnimExt&& o) noexcept {
        std::swap(this->actions, o.actions);
        std::swap(this->shadowX, o.shadowX);
        std::swap(this->shadowY, o.shadowY);
        std::swap(this->shadowScale, o.shadowScale);
        return *this;
    }
    File_pathway::File_pathway(File_pathway&& o) noexcept {
        this->operator=(std::move(o));
    }
    File_pathway& File_pathway::operator=(File_pathway&& o) noexcept {
        std::swap(this->isLoop, o.isLoop);
        std::swap(this->points, o.points);
        return *this;
    }
}
