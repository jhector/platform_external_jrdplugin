#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "nsEngineeringPlugin.h"

int read_sysfile(const char *path, const char **out)
{
	int rv, fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;

	off_t size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	*out = calloc(1, size+1);
	if (!*out)
		return -2;

	if ((rv = read(fd, *out, size)) < 0)
		return rv;

	close(fd);
	return rv;
}

int common_battery_temp(const char **out)
{
	if (read_sysfile("/sys/class/power_supply/battery/temp", out) < 0)
		return PLUGIN_ERROR;

	return PLUGIN_OK;
}

int handler(const char *name, const char *value, const char **out, int mode)
{
	if (mode == PLUGIN_HANDLE_GET) {
		if (strcmp(name, "JrdSrv:Common:BatteryTemp") == 0) {
			return common_battery_temp(out);
		}

		if (strcmp(name, "JrdSrv:StaticTest") == 0) {
			*out = "Static test";
			return PLUGIN_OK;
		}
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
