#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "nsEngineeringPlugin.h"

int read_sysfile(const char *path, const char **out)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;

	off_t size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	*out = calloc(1, size);
	if (!*out)
		return -2;

	return read(fd, *out, size);
}

int common_battery_temp(const char **out)
{
	if (read_sysfile("/system/etc/wifi/macaddr", out) < 0)
		return PLUGIN_ERROR;

	return PLUGIN_OK;
}

int handler(const char *name, const char *value, const char **out, int mode)
{
	if (strcmp(name, "JrdSrv:Common:BatteryTemp") == 0 &&
		mode == PLUGIN_HANDLE_GET) {
		return common_battery_temp(out);
	}

	return PLUGIN_OK;
}

int PluginInit(HostInterface* interface)
{
	if (!interface)
		return PLUGIN_ERROR;

	// register "JrdSrv:xxx" namespace
	if (interface->mRegisterNamespace("JrdSrv", &handler) != PLUGIN_OK) {
		return PLUGIN_ERROR;
	}

	return PLUGIN_OK;
}

void PluginDestroy()
{
}
