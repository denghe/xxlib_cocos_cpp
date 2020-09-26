#pragma once
AJSON(FileExts::LockPoint, x, y);
AJSON(FileExts::CDCircle, x, y, r);
AJSON(FileExts::TimePoint_LockPoints, time, mainLockPoint, lockPoints);
AJSON(FileExts::TimePoint_CDCircles, time, maxCDCircle, cdCircles);
AJSON(FileExts::TimePoint_Speed, time, speed);
AJSON(FileExts::TimePoint_Frame, time, picName);
AJSON(FileExts::Action_Frames, name, totalSeconds, frames);
AJSON(FileExts::Action_AnimExt, name, totalSeconds, lps, cds, ss);
AJSON(FileExts::PathwayPoint, x, y, tension, numSegments);
AJSON(FileExts::File_Frames, actions, plists);
AJSON(FileExts::File_AnimExt, actions, shadowX, shadowY, shadowScale);
AJSON(FileExts::File_pathway, isLoop, points);
