#include "sysinfolinuximpl.h"
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <QFile>
SysInfoLinuxImpl::SysInfoLinuxImpl()
{
}

double SysInfoLinuxImpl::memoryUsed(){
	struct sysinfo memInfo;
	sysinfo(&memInfo);
	qulonglong totalMemory = memInfo.totalram;
	totalMemory += memInfo.totalswap;
	totalMemory *= memInfo.mem_unit;
	qulonglong totalMemoryUsed = memInfo.totalram - memInfo.freeram;
	totalMemoryUsed += memInfo.totalswap - memInfo.freeswap;
	totalMemoryUsed *= memInfo.mem_unit;
	double percent = (double)totalMemoryUsed /
	(double)totalMemory * 100.0;
	return qBound(0.0, percent, 100.0);
}

void SysInfoLinuxImpl::init()
{
	mCpuLoadLastValues = cpuRawData();
}

double SysInfoLinuxImpl::cpuLoadAverage(){
	QVector<qulonglong> firstData = mCpuLoadLastValues;
	QVector<qulonglong> secondData = cpuRawData();
	double overall = (secondData[0] - firstData[0]) + (secondData[1] - firstData[1]) + (secondData[2] - firstData[2]);
	double total = overall + (secondData[3] - firstData[3]);
	mCpuLoadLastValues = secondData;
    return qBound(0.0,overall*100.0/total,100.0);
}


QVector<qulonglong> SysInfoLinuxImpl::cpuRawData()
{
	QFile file("/proc/stat");
	file.open(QIODevice::ReadOnly);
	QByteArray line = file.readLine();
	file.close();
	qulonglong totalUser = 0, totalUserNice = 0,
	totalSystem = 0, totalIdle = 0;
	std::sscanf(line.data(), "cpu %llu %llu %llu %llu", &totalUser, &totalUserNice, &totalSystem, &totalIdle);
	QVector<qulonglong> rawData;
	rawData.append(totalUser);
	rawData.append(totalUserNice);
	rawData.append(totalSystem);
	rawData.append(totalIdle);
	return rawData;
}
