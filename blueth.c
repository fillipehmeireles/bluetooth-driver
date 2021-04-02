/* 

    Bluetooth Driver.

    Scanner.
    ToDo:
        Connect with bluetooth devices.
        Send and receive files.

*/

#include <stdio.h>
#include <stdlib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX_SCAN_SIZE 127

int main()
{
    inquiry_info *scan_info;

    int deviceID = hci_get_route(NULL); // ID of bluetooth driver on machine
    if (deviceID == -1)
    {
        perror("Please, turn on bluetooth...");
        return -1;
    }

    int sock = hci_open_dev(deviceID); // Create Socket to driver by ID
    if (sock == -1)
    {
        perror("Could not create a socket: ");
        return -1;
    }

    scan_info = (inquiry_info *)malloc(MAX_SCAN_SIZE * sizeof(inquiry_info)); //Set max scan size

    int scanned_ble = hci_inquiry(deviceID, 5, MAX_SCAN_SIZE, NULL, &scan_info, IREQ_CACHE_FLUSH); // Scan
    if (scanned_ble == -1)
    {
        perror("Could not scan bluetooth devices");
    }

    printf("%d Bluetooth devices detected\n", scanned_ble);

    char ble_addr_buffer[20];  // Device physical address
    char ble_name_buffer[255]; // Device name

    for (int i = 0; i < scanned_ble; i++)
    {
        ba2str(&(scan_info + i)->bdaddr, ble_addr_buffer);                                                                           // Copy device physical address to buffer
        int scanned_device_name = hci_read_remote_name(sock, &(scan_info + i)->bdaddr, sizeof(ble_name_buffer), ble_name_buffer, 0); // Copy device name to buffer
        if (scanned_device_name < 0)
        {
            strcpy(ble_name_buffer, "[unknown device name]"); // Copy device name to buffer
        }

        printf("%s - %s \n", ble_addr_buffer, ble_name_buffer);
    }

    return 0;
}