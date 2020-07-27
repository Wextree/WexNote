# Oshi 获取对应硬件和操作系统信息

```xml
<dependency>
    <groupId>com.github.oshi</groupId>
    <artifactId>oshi-core</artifactId>
    <version>4.0.0</version>
</dependency>
```




```java
import oshi.SystemInfo;
import oshi.hardware.*;
import oshi.software.os.*;

import java.util.ArrayList;
import java.util.List;

public class SystemInfoTest {
    public static <VirtualMemory> void main(String[] args) {
        // 初始化信息系统对象
        SystemInfo systemInfo = new SystemInfo();
        // 获取硬件信息
        HardwareAbstractionLayer hardware = systemInfo.getHardware();
        // 获取操作系统进程相关信息
        OperatingSystem operatingSystem = systemInfo.getOperatingSystem();

        // 操作BIOS系统信息
        ComputerSystem computerSystem = hardware.getComputerSystem();
        Firmware firmware = computerSystem.getFirmware();
        // 固件名  3ZCN32WW
        String name = firmware.getName();
        // 固件描述  3ZCN32WW
        String description = firmware.getDescription();
        // 经销商  LENOVO - 0
        String firmwareManufacturer = firmware.getManufacturer();
        // 发行时间  2017-06-16
        String releaseDate = firmware.getReleaseDate();
        // 版本  LENOVO - 0
        String firmwareVersion = firmware.getVersion();

        // 传感器信息
        Sensors sensors = hardware.getSensors();
        // 传感器风扇转速 0
        int[] sensorsFanSpeeds = sensors.getFanSpeeds();
        // 传感器电压 0.0
        double sensorsCpuVoltage = sensors.getCpuVoltage();
        // 传感器Cpu温度  50.0
        double sensorsCpuTemperature = sensors.getCpuTemperature();

        // 内存信息
        GlobalMemory memory = hardware.getMemory();
        // 总内存大小  8282333184
        long memoryTotal = memory.getTotal();
        // 可用内存大小  1405657088
        long memoryAvailable = memory.getAvailable();
        // 内存页大小  4096
        long memoryPageSize = memory.getPageSize();

        // CPU线程信息  获取机器硬件执行，cpu频率/型号/核心相关信息
        CentralProcessor processor = hardware.getProcessor();

        // 显示器信息 显示器相关型号/分辨率之类信息
        Display[] displays = hardware.getDisplays();

        // 磁盘硬件信息  当前磁盘的硬件信息，读写状态，分区信息等
        HWDiskStore[] diskStores = hardware.getDiskStores();

        // 网卡信息  获取网卡详细信息，mac/ip4/6地址，读写状态，中断/错误等信息
        NetworkIF[] networkIFS = hardware.getNetworkIFs();

        // 电源状态
        PowerSource[] powerSources = hardware.getPowerSources();

        // 声卡信息
        SoundCard[] soundCards = hardware.getSoundCards();

        // USB  获取USB接口信息，可以过滤出正在使用的USB接口，及相关详细信息，true 树形返回
        UsbDevice[] usbDevices = hardware.getUsbDevices(true);

        // 系统进程相关信息
        // 获取操作系统位数（23/64）
        int bitness = operatingSystem.getBitness();

        // 传入父进程ID，设置返回进程数量及进程排序方法，返回该父进程下指定数量排序下的子进程数
        OSProcess[] childProcesses = operatingSystem.getChildProcesses(1, 2, OperatingSystem.ProcessSort.CPU);

        // 获取操作系统类别  Windows
        String family = operatingSystem.getFamily();

        // 获取文件系统分区信息，剩余空间，挂载信息等 系统存储状态
        FileSystem fileSystem = operatingSystem.getFileSystem();
        OSFileStore[] fileStores = fileSystem.getFileStores();

        // 系统生产厂家
        String manufacturer = operatingSystem.getManufacturer();

        // 系统中网卡信息，dns信息，域名信息，ip4/6信息
        NetworkParams networkParams = operatingSystem.getNetworkParams();
        String[] dnsServers = networkParams.getDnsServers();
        String domainName = networkParams.getDomainName();
        String hostName = networkParams.getHostName();
        String ipv4DefaultGateway = networkParams.getIpv4DefaultGateway();
        String ipv6DefaultGateway = networkParams.getIpv6DefaultGateway();

        // 传入进程号，获取该进程详细信息，所属组/用户/状态等
        OSProcess process = operatingSystem.getProcess(1121);

        // 获取当前系统中所有进程数
        int processCount = operatingSystem.getProcessCount();

        // 根据排序规则 返回指定线程数
        OSProcess[] processes = operatingSystem.getProcesses(10, OperatingSystem.ProcessSort.CPU);

        // 返回指定进程ID下所有进程实例
        List<OSProcess> processList = operatingSystem.getProcesses(new ArrayList<>());

        // 获取系统内所有线程句柄数
        int threadCount = operatingSystem.getThreadCount();

        // 获取系统版本信息
        OperatingSystemVersion version = operatingSystem.getVersion();
    }
}
```