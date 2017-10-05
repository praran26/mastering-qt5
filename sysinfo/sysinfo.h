#ifndef SYSINFO_H
#define SYSINFO_H


class SysInfo
{
public:
	static SysInfo& instance();
    virtual ~SysInfo();
    virtual void init()=0;
    virtual double cpuLoadAverage()=0;
    virtual double memoryUsed()=0;
    SysInfo(const SysInfo& rhs)=delete;
    SysInfo& operator=(const SysInfo& rhs)=delete;
protected:
	explicit SysInfo();
private:
};

#endif // SYSINFO_H