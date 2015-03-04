#include <stdio.h>

#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>

int main(void) {

	int error = 0;
	struct UPNPDev *upnp_dev = upnpDiscover(50, NULL, NULL, 0, 0, &error);

	char lan_address[64];
	struct UPNPUrls upnp_urls;
	struct IGDdatas upnp_data;
	int status = UPNP_GetValidIGD(upnp_dev, &upnp_urls, &upnp_data, lan_address, sizeof(lan_address));
	if (status != 1)
		return -1;

	char wan_address[64];
	UPNP_GetExternalIPAddress(upnp_urls.controlURL, upnp_data.first.servicetype, wan_address);
	printf("%s\n", wan_address);

	unsigned int down = 0, up = 0;
	UPNP_GetLinkLayerMaxBitRates(upnp_urls.controlURL, upnp_data.first.servicetype, &down, &up);
	printf("down: %d up: %d\n", down, up);

	char type[64] = { 0 };
	UPNP_GetConnectionTypeInfo(upnp_urls.controlURL, upnp_data.first.servicetype, type);
	printf("%s\n", type);

	UNSIGNED_INTEGER bytes_send = UPNP_GetTotalBytesSent(upnp_urls.controlURL, upnp_data.first.servicetype);
	UNSIGNED_INTEGER bytes_recv = UPNP_GetTotalBytesReceived(upnp_urls.controlURL, upnp_data.first.servicetype);
	UNSIGNED_INTEGER packets_send = UPNP_GetTotalPacketsSent(upnp_urls.controlURL, upnp_data.first.servicetype);
	UNSIGNED_INTEGER packets_recv = UPNP_GetTotalPacketsReceived(upnp_urls.controlURL, upnp_data.first.servicetype);
	printf("send: %llu bytes = %llu packets\nrecv: %llu bytes = %llu packets\n", bytes_send, packets_send, bytes_recv, packets_recv);

	FreeUPNPUrls(&upnp_urls);
	freeUPNPDevlist(upnp_dev);
}
