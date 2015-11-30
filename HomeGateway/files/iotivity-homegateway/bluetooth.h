/*
 * Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
 * Copyright (c) 2013-2014 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_NETWORK_BLUETOOTH_H__
#define __TIZEN_NETWORK_BLUETOOTH_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef TIZEN
#include <tizen_error.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file        bluetooth.h
 * @brief       API to control the Bluetooth adapter and devices and communications.
 * @ingroup     CAPI_NETWORK_BLUETOOTH_MODULE
 */


/**
 * @addtogroup CAPI_NETWORK_BLUETOOTH_MODULE
 * @{
 */

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Enumerations of connected Bluetooth device event role.
 */
typedef enum
{
	BT_SOCKET_UNKNOWN = 0x00, /**< Unknown role*/
	BT_SOCKET_SERVER , /**< Server role*/
	BT_SOCKET_CLIENT, /**< Client role*/
} bt_socket_role_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 * @brief Enumerations of Bluetooth error codes.
 */
typedef enum
{
#ifdef TIZEN
	BT_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful*/
	BT_ERROR_CANCELLED = TIZEN_ERROR_CANCELED, /**< Operation cancelled */
	BT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	BT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	BT_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**< Device or resource busy */
	BT_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Timeout error */
	BT_ERROR_NOW_IN_PROGRESS = TIZEN_ERROR_NOW_IN_PROGRESS, /**< Operation now in progress */
	BT_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NETWORK_CLASS|0x111, /**< Not Supported */
	BT_ERROR_NOT_INITIALIZED = TIZEN_ERROR_NETWORK_CLASS|0x0101, /**< Local adapter not initialized */
	BT_ERROR_NOT_ENABLED = TIZEN_ERROR_NETWORK_CLASS|0x0102, /**< Local adapter not enabled */
	BT_ERROR_ALREADY_DONE = TIZEN_ERROR_NETWORK_CLASS|0x0103, /**< Operation already done  */
	BT_ERROR_OPERATION_FAILED = TIZEN_ERROR_NETWORK_CLASS|0x0104, /**< Operation failed */
	BT_ERROR_NOT_IN_PROGRESS = TIZEN_ERROR_NETWORK_CLASS|0x0105, /**< Operation not in progress */
	BT_ERROR_REMOTE_DEVICE_NOT_BONDED = TIZEN_ERROR_NETWORK_CLASS|0x0106, /**< Remote device not bonded */
	BT_ERROR_AUTH_REJECTED = TIZEN_ERROR_NETWORK_CLASS|0x0107, /**< Authentication rejected */
	BT_ERROR_AUTH_FAILED = TIZEN_ERROR_NETWORK_CLASS|0x0108, /**< Authentication failed */
	BT_ERROR_REMOTE_DEVICE_NOT_FOUND = TIZEN_ERROR_NETWORK_CLASS|0x0109, /**< Remote device not found */
	BT_ERROR_SERVICE_SEARCH_FAILED = TIZEN_ERROR_NETWORK_CLASS|0x010A, /**< Service search failed */
	BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED = TIZEN_ERROR_NETWORK_CLASS|0x010B, /**< Remote device is not connected */
	BT_ERROR_ADAPTER_NOT_FOUND = TIZEN_ERROR_NETWORK_CLASS|0x010C, /**< Adapter not found */
#else
	BT_ERROR_NONE = 0x00, /**< Successful*/
	BT_ERROR_CANCELLED, /**< Operation cancelled */
	BT_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	BT_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	BT_ERROR_RESOURCE_BUSY, /**< Device or resource busy */
	BT_ERROR_TIMED_OUT, /**< Timeout error */
	BT_ERROR_NOW_IN_PROGRESS, /**< Operation now in progress */
	BT_ERROR_NOT_SUPPORTED, /**< Not Supported */
	BT_ERROR_NOT_INITIALIZED, /**< Local adapter not initialized */
	BT_ERROR_NOT_ENABLED, /**< Local adapter not enabled */
	BT_ERROR_ALREADY_DONE, /**< Operation already done  */
	BT_ERROR_OPERATION_FAILED, /**< Operation failed */
	BT_ERROR_NOT_IN_PROGRESS, /**< Operation not in progress */
	BT_ERROR_REMOTE_DEVICE_NOT_BONDED, /**< Remote device not bonded */
	BT_ERROR_AUTH_REJECTED, /**< Authentication rejected */
	BT_ERROR_AUTH_FAILED, /**< Authentication failed */
	BT_ERROR_REMOTE_DEVICE_NOT_FOUND, /**< Remote device not found */
	BT_ERROR_SERVICE_SEARCH_FAILED, /**< Service search failed */
	BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED, /**< Remote device is not connected */
	BT_ERROR_ADAPTER_NOT_FOUND, /**< Adapter not found */
#endif
} bt_error_e;

#define BT_SUCCESS BT_ERROR_NONE

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of device disconnect reason.
 * @since_tizen 2.3
 */
typedef enum
{
	BT_DEVICE_DISCONNECT_REASON_UNKNOWN, /**< Disconnected by unknown reason */
	BT_DEVICE_DISCONNECT_REASON_TIMEOUT, /**< Disconnected by timeout */
	BT_DEVICE_DISCONNECT_REASON_LOCAL_HOST, /**< Disconnected by local host */
	BT_DEVICE_DISCONNECT_REASON_REMOTE, /**< Disconnected by remote */
} bt_device_disconnect_reason_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of connection link type.
 * @since_tizen 2.3
 */
typedef enum
{
	BT_DEVICE_CONNECTION_LINK_BREDR, /**< BR/EDR link */
	BT_DEVICE_CONNECTION_LINK_LE, /**< LE link */
	BT_DEVICE_CONNECTION_LINK_DEFAULT = 0xFF, /**< The connection type defualt */
} bt_device_connection_link_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device connection information structure.
 * @since_tizen 2.3
 *
 * @see bt_device_connection_state_changed_cb()
 */
typedef struct
{
	char *remote_address;   /**< The address of remote device */
	bt_device_connection_link_type_e link;  /**< Link type */
	bt_device_disconnect_reason_e disconn_reason;  /**< Disconnection reason */
} bt_device_connection_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Enumerations of the Bluetooth adapter state.
 */
typedef enum
{
	BT_ADAPTER_DISABLED = 0x00, /**< Bluetooth adapter is disabled */
	BT_ADAPTER_ENABLED, /**< Bluetooth adapter is enabled */
} bt_adapter_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumerations of the Bluetooth visibility mode.
 */
typedef enum
{
	BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE = 0x00,  /**< Other devices cannot find your device via discovery */
	BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE,  /**< Discoverable mode */
	BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE,  /**< Discoverable mode with time limit. After specific period,
							    it is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE.*/
} bt_adapter_visibility_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumerations of the discovery state of Bluetooth device.
 * @since_tizen 2.3
 */
typedef enum
{
	BT_ADAPTER_DEVICE_DISCOVERY_STARTED, /**< Device discovery is started */
	BT_ADAPTER_DEVICE_DISCOVERY_FINISHED, /**< Device discovery is finished */
	BT_ADAPTER_DEVICE_DISCOVERY_FOUND, /**< The remote Bluetooth device is found */
} bt_adapter_device_discovery_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of device authorization state.
 */
typedef enum
{
	BT_DEVICE_AUTHORIZED, /**< The remote Bluetooth device is authorized */
	BT_DEVICE_UNAUTHORIZED, /**< The remote Bluetooth device is unauthorized */
} bt_device_authorization_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of Bluetooth profile.
 */
typedef enum
{
	BT_PROFILE_RFCOMM = 0x01, /**< RFCOMM Profile */
	BT_PROFILE_A2DP = 0x02, /**< Advanced Audio Distribution Profile */
	BT_PROFILE_HSP = 0x04, /**< Headset Profile */
	BT_PROFILE_HID = 0x08, /**< Human Interface Device Profile */
	BT_PROFILE_NAP = 0x10, /**< Network Access Point Profile */
	BT_PROFILE_AG = 0x20, /**< Audio Gateway Profile */
	BT_PROFILE_GATT = 0x40, /**< Generic Attribute Profile */
	BT_PROFILE_NAP_SERVER = 0x80, /**< NAP server Profile */
} bt_profile_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Enumerations of Bluetooth socket connection state.
 */
typedef enum
{
	BT_SOCKET_CONNECTED, /**< RFCOMM is connected */
	BT_SOCKET_DISCONNECTED, /**< RFCOMM is disconnected */
} bt_socket_connection_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of major service class.
 */
typedef enum
{
	BT_MAJOR_SERVICE_CLASS_LIMITED_DISCOVERABLE_MODE  = 0x002000, /**< Limited discoverable mode */
	BT_MAJOR_SERVICE_CLASS_POSITIONING                = 0x010000, /**< Positioning class */
	BT_MAJOR_SERVICE_CLASS_NETWORKING                 = 0x020000, /**< Networking class */
	BT_MAJOR_SERVICE_CLASS_RENDERING                  = 0x040000, /**< Rendering class */
	BT_MAJOR_SERVICE_CLASS_CAPTURING                  = 0x080000, /**< Capturing class */
	BT_MAJOR_SERVICE_CLASS_OBJECT_TRANSFER            = 0x100000, /**< Object transferring class */
	BT_MAJOR_SERVICE_CLASS_AUDIO                      = 0x200000, /**< Audio class*/
	BT_MAJOR_SERVICE_CLASS_TELEPHONY                  = 0x400000, /**< Telephony class */
	BT_MAJOR_SERVICE_CLASS_INFORMATION                = 0x800000, /**< Information class */
} bt_major_service_class_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of major device class.
 */
typedef enum
{
	BT_MAJOR_DEVICE_CLASS_MISC = 0x00, /**< Miscellaneous major device class*/
	BT_MAJOR_DEVICE_CLASS_COMPUTER = 0x01, /**< Computer major device class */
	BT_MAJOR_DEVICE_CLASS_PHONE = 0x02, /**< Phone major device class */
	BT_MAJOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT = 0x03, /**< LAN/Network access point major device class */
	BT_MAJOR_DEVICE_CLASS_AUDIO_VIDEO = 0x04, /**< Audio/Video major device class */
	BT_MAJOR_DEVICE_CLASS_PERIPHERAL = 0x05, /**< Peripheral major device class */
	BT_MAJOR_DEVICE_CLASS_IMAGING = 0x06, /**< Imaging major device class */
	BT_MAJOR_DEVICE_CLASS_WEARABLE = 0x07, /**< Wearable device class */
	BT_MAJOR_DEVICE_CLASS_TOY = 0x08, /**< Toy device class */
	BT_MAJOR_DEVICE_CLASS_HEALTH = 0x09, /**< Health device class */
	BT_MAJOR_DEVICE_CLASS_UNCATEGORIZED = 0x1F, /**< Uncategorized major device class */
} bt_major_device_class_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of minor device class.
 */
typedef enum
{
	BT_MINOR_DEVICE_CLASS_COMPUTER_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_DESKTOP_WORKSTATION = 0x04, /**< Desktop workstation minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_SERVER_CLASS = 0x08, /**< Server minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_LAPTOP = 0x0C, /**< Laptop minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_HANDHELD_PC_OR_PDA = 0x10, /**< Handheld PC/PDA minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_PALM_SIZED_PC_OR_PDA = 0x14, /**< Palm sized PC/PDA minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_WEARABLE_COMPUTER = 0x18, /**< Wearable(watch sized) minor device class of computer */

	BT_MINOR_DEVICE_CLASS_PHONE_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_CELLULAR = 0x04, /**< Cellular minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_CORDLESS = 0x08, /**< Cordless minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_SMART_PHONE = 0x0C, /**< Smart phone minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_WIRED_MODEM_OR_VOICE_GATEWAY = 0x10, /**< Wired modem or voice gateway minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_COMMON_ISDN_ACCESS = 0x14, /**< Common ISDN minor device class of phone */

	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_FULLY_AVAILABLE = 0x04, /**< Fully available minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_1_TO_17_PERCENT_UTILIZED = 0x20, /**< 1-17% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_17_TO_33_PERCENT_UTILIZED = 0x40, /**< 17-33% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_33_TO_50_PERCENT_UTILIZED = 0x60, /**< 33-50% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_50_to_67_PERCENT_UTILIZED = 0x80, /**< 50-67% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_67_TO_83_PERCENT_UTILIZED = 0xA0, /**< 67-83% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_83_TO_99_PERCENT_UTILIZED = 0xC0, /**< 83-99% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_NO_SERVICE_AVAILABLE = 0xE0, /**< No service available minor device class of LAN/Network access point */

	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_WEARABLE_HEADSET = 0x04, /**< Wearable headset minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HANDS_FREE = 0x08, /**< Hands-free minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_MICROPHONE = 0x10, /**< Microphone minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_LOUDSPEAKER = 0x14, /**< Loudspeaker minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HEADPHONES = 0x18, /**< Headphones minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_PORTABLE_AUDIO = 0x1C, /**< Portable audio minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_CAR_AUDIO = 0x20, /**< Car audio minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_SET_TOP_BOX = 0x24, /**< Set-top box minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HIFI_AUDIO_DEVICE = 0x28, /**< Hifi audio minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VCR = 0x2C, /**< VCR minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_CAMERA = 0x30, /**< Video camera minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_CAMCORDER = 0x34, /**< Camcorder minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_MONITOR = 0x38, /**< Video monitor minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_DISPLAY_LOUDSPEAKER = 0x3C, /**< Video display and loudspeaker minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_CONFERENCING = 0x40, /**< Video conferencing minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_GAMING_TOY = 0x48, /**< Gaming/toy minor device class of audio/video */

	BT_MINOR_DEVICE_CLASS_PERIPHERA_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_KEY_BOARD = 0x40, /**< Key board minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_POINTING_DEVICE = 0x80, /**< Pointing device minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_COMBO_KEYBOARD_POINTING_DEVICE = 0xC0, /**< Combo keyboard or pointing device minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_JOYSTICK = 0x04, /**< Joystick minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_GAME_PAD = 0x08, /**< Game pad minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_REMOTE_CONTROL = 0x0C, /**< Remote control minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_SENSING_DEVICE = 0x10, /**< Sensing device minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITIZER_TABLET = 0x14, /**< Digitizer minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_CARD_READER = 0x18, /**< Card reader minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITAL_PEN = 0x1C, /**< Digital pen minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_SCANNER = 0x20, /**< Handheld scanner minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_GESTURAL_INPUT_DEVICE = 0x24, /**< Handheld gestural input device minor device class of peripheral */

	BT_MINOR_DEVICE_CLASS_IMAGING_DISPLAY = 0x10, /**< Display minor device class of imaging */
	BT_MINOR_DEVICE_CLASS_IMAGING_CAMERA = 0x20, /**< Camera minor device class of imaging */
	BT_MINOR_DEVICE_CLASS_IMAGING_SCANNER = 0x40, /**< Scanner minor device class of imaging */
	BT_MINOR_DEVICE_CLASS_IMAGING_PRINTER = 0x80, /**< Printer minor device class of imaging */

	BT_MINOR_DEVICE_CLASS_WEARABLE_WRIST_WATCH = 0x04, /**< Wrist watch minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_PAGER = 0x08, /**< Pager minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_JACKET = 0x0C, /**< Jacket minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_HELMET = 0x10, /**< Helmet minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_GLASSES = 0x14, /**< Glasses minor device class of wearable */

	BT_MINOR_DEVICE_CLASS_TOY_ROBOT = 0x04, /**< Robot minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_VEHICLE = 0x08, /**< Vehicle minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_DOLL_ACTION = 0x0C, /**< Doll/Action minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_CONTROLLER = 0x10, /**< Controller minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_GAME = 0x14, /**< Game minor device class of toy */

	BT_MINOR_DEVICE_CLASS_HEATH_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_BLOOD_PRESSURE_MONITOR = 0x04, /**< Blood pressure monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_THERMOMETER = 0x08, /**< Thermometer minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_WEIGHING_SCALE = 0x0C, /**< Weighing scale minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_GLUCOSE_METER = 0x10, /**< Glucose minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_PULSE_OXIMETER = 0x14, /**< Pulse oximeter minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_HEART_PULSE_RATE_MONITOR = 0x18, /**< Heart/Pulse rate monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_DATA_DISPLAY = 0x1C, /**< Health data display minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_STEP_COUNTER = 0x20, /**< Step counter minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_BODY_COMPOSITION_ANALYZER = 0x24, /**< Body composition analyzer minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_PEAK_FLOW_MONITOR = 0x28, /**< Peak flow monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_MEDICATION_MONITOR = 0x2C, /**< Medication monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_KNEE_PROSTHESIS = 0x30, /**< Knee prosthesis minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEATH_ANKLE_PROSTHESIS = 0x34, /**< Ankle prosthesis minor device class of health */
} bt_minor_device_class_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Enumerations for the types of PAN(Personal Area Networking) service
 */
typedef enum {
    BT_PANU_SERVICE_TYPE_NAP = 0,  /**< Network Access Point */
} bt_panu_service_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Enumerations for the types of profiles related with audio
 */
typedef enum {
	BT_AUDIO_PROFILE_TYPE_ALL = 0,  /**< All supported profiles related with audio */
	BT_AUDIO_PROFILE_TYPE_HSP_HFP,  /**< HSP(Headset Profile) and HFP(Hands-Free Profile) */
	BT_AUDIO_PROFILE_TYPE_A2DP,  /**< A2DP(Advanced Audio Distribution Profile) */
	BT_AUDIO_PROFILE_TYPE_AG,  /**< AG(Audio Gateway) */
} bt_audio_profile_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the equalizer state
 */
typedef enum {
    BT_AVRCP_EQUALIZER_STATE_OFF = 0x01,  /**< Equalizer Off */
    BT_AVRCP_EQUALIZER_STATE_ON,  /**< Equalizer On */
} bt_avrcp_equalizer_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the repeat mode
 */
typedef enum {
    BT_AVRCP_REPEAT_MODE_OFF = 0x01,  /**< Repeat Off */
    BT_AVRCP_REPEAT_MODE_SINGLE_TRACK,  /**< Single track repeat */
    BT_AVRCP_REPEAT_MODE_ALL_TRACK,  /**< All track repeat */
    BT_AVRCP_REPEAT_MODE_GROUP,  /**< Group repeat */
} bt_avrcp_repeat_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the shuffle mode
 */
typedef enum {
    BT_AVRCP_SHUFFLE_MODE_OFF = 0x01,  /**< Shuffle Off */
    BT_AVRCP_SHUFFLE_MODE_ALL_TRACK,  /**< All tracks shuffle */
    BT_AVRCP_SHUFFLE_MODE_GROUP,  /**< Group shuffle */
} bt_avrcp_shuffle_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the scan mode
 */
typedef enum {
    BT_AVRCP_SCAN_MODE_OFF = 0x01,  /**< Scan Off */
    BT_AVRCP_SCAN_MODE_ALL_TRACK,  /**< All tracks scan */
    BT_AVRCP_SCAN_MODE_GROUP,  /**< Group scan */
} bt_avrcp_scan_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the player state
 */
typedef enum {
    BT_AVRCP_PLAYER_STATE_STOPPED = 0x00,  /**< Stopped */
    BT_AVRCP_PLAYER_STATE_PLAYING,  /**< Playing */
    BT_AVRCP_PLAYER_STATE_PAUSED,  /**< Paused */
    BT_AVRCP_PLAYER_STATE_FORWARD_SEEK,  /**< Seek Forward */
    BT_AVRCP_PLAYER_STATE_REWIND_SEEK,  /**< Seek Rewind */
} bt_avrcp_player_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Enumerations for the data channel type
 */
typedef enum {
    BT_HDP_CHANNEL_TYPE_RELIABLE  = 0x01,  /**< Reliable Data Channel */
    BT_HDP_CHANNEL_TYPE_STREAMING,  /**< Streaming Data Channel */
} bt_hdp_channel_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 */
typedef enum {
    BT_AG_CALL_EVENT_IDLE = 0x00,  /**< Idle */
    BT_AG_CALL_EVENT_ANSWERED,  /**< Answered */
    BT_AG_CALL_EVENT_HELD,  /**< Held */
    BT_AG_CALL_EVENT_RETRIEVED,  /**< Retrieved */
    BT_AG_CALL_EVENT_DIALING,  /**< Dialing */
    BT_AG_CALL_EVENT_ALERTING,  /**< Alerting */
    BT_AG_CALL_EVENT_INCOMING,  /**< Incoming */
} bt_ag_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 */
typedef enum {
    BT_AG_CALL_STATE_IDLE = 0x00,  /**< Idle state */
    BT_AG_CALL_STATE_ACTIVE,  /**< Active state */
    BT_AG_CALL_STATE_HELD,  /**< Held state */
    BT_AG_CALL_STATE_DIALING,  /**< Dialing state */
    BT_AG_CALL_STATE_ALERTING,  /**< Alerting state */
    BT_AG_CALL_STATE_INCOMING,  /**< Incoming state */
    BT_AG_CALL_STATE_WAITING,  /**< Waiting for connected indication event after answering an incoming call*/
} bt_ag_call_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call handling event
 */
typedef enum {
    BT_AG_CALL_HANDLING_EVENT_ANSWER = 0x00,  /**< Request to answer an incoming call */
    BT_AG_CALL_HANDLING_EVENT_RELEASE,  /**< Request to release a call */
    BT_AG_CALL_HANDLING_EVENT_REJECT,  /**< Request to reject an incoming call */
} bt_ag_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the multi call handling event
 */
typedef enum {
    BT_AG_MULTI_CALL_HANDLING_EVENT_RELEASE_HELD_CALLS = 0x00,  /**< Request to release held calls */
    BT_AG_MULTI_CALL_HANDLING_EVENT_RELEASE_ACTIVE_CALLS,  /**< Request to release active calls */
    BT_AG_MULTI_CALL_HANDLING_EVENT_ACTIVATE_HELD_CALL,  /**< Request to put active calls into hold state and activate another (held or waiting) call */
    BT_AG_MULTI_CALL_HANDLING_EVENT_MERGE_CALLS,  /**< Request to add a held call to the conversation */
    BT_AG_MULTI_CALL_HANDLING_EVENT_EXPLICIT_CALL_TRANSFER,  /**< Request to let a user who has two calls to connect these two calls together and release its connections to both other parties */
} bt_ag_multi_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  The handle of calls state
 */
typedef void* bt_call_list_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  The attribute handle of GATT(Generic Attribute Profile)
 */
typedef void* bt_gatt_attribute_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Class structure of device and service.
 *
 * @see #bt_device_info_s
 * @see #bt_adapter_device_discovery_info_s
 * @see bt_device_bond_created_cb()
 * @see bt_adapter_device_discovery_state_changed_cb()
 */
typedef struct
{
	bt_major_device_class_e major_device_class;	/**< Major device class. */
	bt_minor_device_class_e minor_device_class;	/**< Minor device class. */
	int major_service_class_mask;	/**< Major service class mask.
	This value can be a combination of #bt_major_service_class_e like #BT_MAJOR_SERVICE_CLASS_RENDERING | #BT_MAJOR_SERVICE_CLASS_AUDIO */
} bt_class_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of service class.
 * @since_tizen 2.3
 */
typedef enum {
	BT_SC_NONE = 0, /**< No service class */
	BT_SC_RES_SERVICE_MASK = 0x00000001, /**< RES service class */
	BT_SC_SPP_SERVICE_MASK = 0x00000002, /**< SPP nervice class */
	BT_SC_DUN_SERVICE_MASK = 0x00000004, /**< DUN service class */
	BT_SC_FAX_SERVICE_MASK = 0x00000008, /**< FAX service class */
	BT_SC_LAP_SERVICE_MASK = 0x00000010, /**< LAP service class */
	BT_SC_HSP_SERVICE_MASK = 0x00000020, /**< HSP service class */
	BT_SC_HFP_SERVICE_MASK = 0x00000040, /**< HFP service class */
	BT_SC_OPP_SERVICE_MASK = 0x00000080, /**< OPP service class */
	BT_SC_FTP_SERVICE_MASK = 0x00000100, /**< FTP service class */
	BT_SC_CTP_SERVICE_MASK = 0x00000200, /**< CTP service class */
	BT_SC_ICP_SERVICE_MASK = 0x00000400, /**< ICP service class */
	BT_SC_SYNC_SERVICE_MASK = 0x00000800, /**< SYNC service class */
	BT_SC_BPP_SERVICE_MASK = 0x00001000, /**< BPP service class */
	BT_SC_BIP_SERVICE_MASK = 0x00002000, /**< BIP service class */
	BT_SC_PANU_SERVICE_MASK = 0x00004000, /**< PANU service class */
	BT_SC_NAP_SERVICE_MASK = 0x00008000, /**< NAP service class */
	BT_SC_GN_SERVICE_MASK = 0x00010000, /**< GN service class */
	BT_SC_SAP_SERVICE_MASK = 0x00020000, /**< SAP service class */
	BT_SC_A2DP_SERVICE_MASK = 0x00040000, /**< A2DP service class */
	BT_SC_AVRCP_SERVICE_MASK = 0x00080000, /**< AVRCP service class */
	BT_SC_PBAP_SERVICE_MASK = 0x00100000, /**< PBAP service class */
	BT_SC_HID_SERVICE_MASK = 0x00200000, /**< HID service class */
	BT_SC_ALL_SERVICE_MASK = 0x00FFFFFF, /**< ALL service class */
	BT_SC_MAX /**< MAX service class */
} bt_service_class_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of gap appearance type.
 * @since_tizen 2.3
 */
typedef enum {
	BT_APPEARANCE_TYPE_UNKNOWN = 0x00, /**< Unknown appearance type */
	BT_APPEARANCE_TYPE_GENERIC_PHONE = 0x40, /**< Generic Phone type - Generic category */
	BT_APPEARANCE_TYPE_GENERIC_COMPUTER = 0x80, /**< Generic Computer type - Generic category */
	BT_APPEARANCE_TYPE_GENERIC_WATCH = 0xC0, /**< Generic Watch type - Generic category */
} bt_appearance_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Structure of device discovery information.
 * @since_tizen 2.3
 *
 * @see #bt_class_s
 * @see bt_adapter_device_discovery_state_changed_cb()
 */
typedef struct
{
	char *remote_address;   /**< The address of remote device */
	char *remote_name;      /**< The name of remote device */
	bt_class_s bt_class;    /**< The Bluetooth classes */
	int rssi;       /**< The strength indicator of received signal  */
	bool is_bonded; /**< The bonding state */
	char **service_uuid;  /**< The UUID list of service */
	int service_count;      /**< The number of services */
	bt_appearance_type_e appearance;        /**< The Bluetooth appearance */
	int manufacturer_data_len;      /**< manufacturer specific data length */
	char *manufacturer_data;                /**< manufacturer specific data */
} bt_adapter_device_discovery_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device information structure used for identifying pear device.
 *
 * @see #bt_class_s
 * @see bt_device_bond_created_cb()
 */
typedef struct
{
	char *remote_address;	/**< The address of remote device */
	char *remote_name;	/**< The name of remote device */
	bt_class_s bt_class;    /**< The Bluetooth classes */
	char **service_uuid;  /**< The UUID list of service */
	int service_count;	/**< The number of services */
	bool is_bonded;	/**< The bonding state */
	bool is_connected;	/**< The connection state */
	bool is_authorized;	/**< The authorization state */
} bt_device_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Service Discovery Protocol (SDP) data structure.
 *
 * @details This protocol is used for discovering available services or pear device,
 * and finding one to connect with.
 *
 * @see bt_device_service_searched_cb()
 */
typedef struct
{
	char *remote_address;   /**< The address of remote device */
	char **service_uuid;  /**< The UUID list of service */
	int service_count;    /**< The number of services. */
} bt_device_sdp_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 *
 * @brief Rfcomm connection data used for exchanging data between Bluetooth devices.
 *
 * @see bt_socket_connection_state_changed_cb()
 */
typedef struct
{
	int socket_fd;	/**< The file descriptor of connected socket */
	int server_fd;  /**< The file descriptor of the server socket or -1 for client connection */
	bt_socket_role_e local_role;	/**< The local device role in this connection */
	char *remote_address;	/**< The remote device address */
	char *service_uuid;	/**< The service UUId */
} bt_socket_connection_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 *
 * @brief Structure of RFCOMM received data.
 *
 * @remarks User can use standard linux functions for reading/writing
 * data from/to sockets.
 *
 * @see bt_socket_data_received_cb()
 */
typedef struct
{
	int socket_fd;	/**< The socket fd */
	int data_size;	/**< The length of the received data */
	char *data;	/**< The received data */
} bt_socket_received_data_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 *
 * @brief Structure of agent callbacks.
 *
 * @see bt_agent_register()
 */
typedef void bt_req_t;

typedef struct
{
	void (*display_pincode)(const char *device_name,
				const char *pincode,
				bt_req_t *requestion);
	void (*request_pincode)(const char *device_name,
				bt_req_t *requestion);
	void (*request_passkey)(const char *device_name,
				bt_req_t *requestion);
	void (*display_passkey)(const char *device_name,
				guint32 passkey,
				guint16 entered,
				bt_req_t *requestion);
	void (*request_confirm)(const char *device_name,
				unsigned int confirm_num,
				bt_req_t *requestion);
	void (*authorize_service)(const char *device_name,
				const char *uuid,
				bt_req_t *requestion);
	void (*cancel)(void);
} bt_agent;

#ifdef TIZEN_3
/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 *
 * @brief  Enumerations used by user for synchronous agent replies
 *
 * @see bt_agent_reply_sync()
 */
typedef enum
{
  BT_AGENT_ACCEPT = 0, /**< accept user request over bt_agent */
  BT_AGENT_REJECT, /**< reject user request over bt_agent */
} bt_agent_accept_type_t;
#endif

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the Bluetooth adapter state changes.
 * @param[in]   result  The result of the adapter state changing
 * @param[in]   adapter_state  The adapter state to be changed
 * @param[in]   user_data  The user data passed from the callback registration function
 * @pre Either bt_adapter_enable() or bt_adapter_disable() will invoke this callback if you register this callback using bt_adapter_set_state_changed_cb().
 * @see bt_adapter_enable()
 * @see bt_adapter_disable()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 */
typedef void (*bt_adapter_state_changed_cb)(int result, bt_adapter_state_e adapter_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when adapter name changes.
 * @param[in]   device_name	The name of the Bluetooth device to be changed
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when the name of Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_name_changed_cb().
 * @see bt_adapter_set_name()
 * @see bt_adapter_set_name_changed_cb()
 * @see bt_adapter_unset_name_changed_cb()
 */
typedef void (*bt_adapter_name_changed_cb)(char *device_name, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the visibility mode changes.
 * @param[in] result The result of the visibility mode changing
 * @param[in] visibility_mode The visibility mode to be changed
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre This function will be invoked when the visibility of Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_visibility_mode_changed_cb().
 *
 * @see bt_adapter_set_visibility()
 * @see bt_adapter_set_visibility_mode_changed_cb()
 * @see bt_adapter_unset_visibility_mode_changed_cb()
 */
typedef void (*bt_adapter_visibility_mode_changed_cb)
	(int result, bt_adapter_visibility_mode_e visibility_mode, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called every second until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE.
 * @remarks  This callback function is called only if visibility mode is #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE.
 * @param[in]  duration  The duration until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE (in seconds)
 * @param[in]  user_data  The user data passed from the callback registration function
 * @pre  This function will be invoked if you register this callback using bt_adapter_set_visibility_duration_changed_cb().
 * @see  bt_adapter_set_visibility_duration_changed_cb()
 * @see  bt_adapter_unset_visibility_duration_changed_cb()
 * @see  bt_adapter_set_visibility()
 */
typedef void (*bt_adapter_visibility_duration_changed_cb)(int duration, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the state of device discovery changes.
 *
 * @remarks If \a discovery_state is #BT_ADAPTER_DEVICE_DISCOVERY_FOUND,
 * then you can get some information, such as remote device address, remote device name, rssi, and bonding state.
 *
 * @param[in] result The result of the device discovery
 * @param[in] discovery_state The discovery state to be changed
 * @param[in] device_info The information of the discovered device \n
 *					If \a discovery_state is #BT_ADAPTER_DEVICE_DISCOVERY_STARTED or
 * #BT_ADAPTER_DEVICE_DISCOVERY_FINISHED, then \a device_info is NULL.
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre Either bt_adapter_start_device_discovery() or bt_adapter_stop_device_discovery() will invoke this function
 * if you register this callback using bt_adapter_set_device_discovery_state_changed_cb().
 *
 * @see bt_adapter_start_device_discovery()
 * @see bt_adapter_stop_device_discovery()
 * @see bt_adapter_set_device_discovery_state_changed_cb()
 * @see bt_adapter_unset_device_discovery_state_changed_cb()
 *
 */
typedef void (*bt_adapter_device_discovery_state_changed_cb)
	(int result, bt_adapter_device_discovery_state_e discovery_state, bt_adapter_device_discovery_info_s *discovery_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when you get bonded devices repeatedly.
 *
 * @param[in] device_info The bonded device information
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre bt_adapter_foreach_bonded_device() will invoke this function.
 *
 * @see bt_adapter_foreach_bonded_device()
 *
 */
typedef bool (*bt_adapter_bonded_device_cb)(bt_device_info_s *device_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Called when the process of creating bond finishes.
 * @remarks If the remote user does not respond within 60 seconds, a time out will happen with #BT_ERROR_TIMED_OUT result code.\n
 * If bt_device_cancel_bonding() is called and it returns #BT_ERROR_NONE, then this callback function will be called
 * with #BT_ERROR_CANCELLED result. \n
 * If creating a bond succeeds but service search fails, then this callback will be called with #BT_ERROR_SERVICE_SEARCH_FAILED.
 * In this case, you should try service search again by bt_device_start_service_search() to get the supported service list.
 *
 * @param[in] result The result of the bonding device
 * @param[in] device_info The device information which you creates bond with
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre Either bt_device_create_bond() will invoke this function
 * if you register this callback using bt_device_set_bond_created_cb().
 *
 * @see bt_device_create_bond()
 * @see bt_device_cancel_bonding()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
typedef void (*bt_device_bond_created_cb)(int result, bt_device_info_s *device_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when you get connected profiles repeatedly.
 * @param[in] profile The connected Bluetooth profile
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre bt_device_foreach_connected_profiles() will invoke this function.
 * @see bt_device_foreach_connected_profiles()
 */
typedef bool (*bt_device_connected_profile)(bt_profile_e profile, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the bond destroys.
 * @param[in] result The result that a bond is destroyed
 * @param[in] remote_address The address of the remote Bluetooth device to destroy bond with
 * @param[in] user_data The user data passed from the callback registration function
 * @pre bt_device_destroy_bond() will invoke this function
 * if you register this callback using bt_device_set_bond_destroyed_cb().
 *
 * @see bt_device_destroy_bond()
 * @see bt_device_set_bond_destroyed_cb()
 * @see bt_device_unset_bond_destroyed_cb()
 */
typedef void (*bt_device_bond_destroyed_cb)(int result, char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the authorization of device changes.
 * @param[in] authorization The authorization of device
 * @param[in] remote_address The address of the remote Bluetooth device which is (un)authorized
 * @param[in] user_data The user data passed from the callback registration function
 * @pre bt_device_set_authorization() will invoke this function if you register this callback using bt_device_set_authorization_changed_cb().
 *
 * @see bt_device_set_authorization()
 * @see #bt_device_set_authorization_changed_cb()
 * @see #bt_device_unset_authorization_changed_cb()
 */
typedef void (*bt_device_authorization_changed_cb)
	(bt_device_authorization_e authorization, char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the process of service search finishes.
 * @remark If bt_device_cancel_service_search() is called and it returns #BT_ERROR_NONE,
 * then this callback function will be called with #BT_ERROR_CANCELLED result.
 *
 * @param[in] result The result of the service searching
 * @param[in] sdp_info The structure of service lists found on a device
 * @param[in] user_data The user data passed from the callback registration function
 * @pre Either bt_device_start_service_search() will invoke this function
 * if you register this callback using  bt_device_set_service_searched_cb().
 *
 * @see bt_device_start_service_search()
 * @see bt_device_cancel_service_search()
 * @see bt_device_set_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 *
 */
typedef void (*bt_device_service_searched_cb)(int result, bt_device_sdp_info_s *sdp_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen 2.3
 *
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] conn_info The connection information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_device_set_connection_state_changed_cb()
 * @see bt_device_unset_connection_state_changed_cb()
 */
typedef void (*bt_device_connection_state_changed_cb)(bool connected, bt_device_connection_info_s *conn_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Called when you receive data.
 *
 * @param[in] data The received data from the remote device
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre When the connected remote Bluetooth device invokes bt_socket_send_data(),
 *		this function will be invoked if you register this function using bt_socket_set_data_received_cb().
 *
 * @see bt_socket_set_data_received_cb()
 * @see bt_socket_unset_data_received_cb()
 * @see bt_socket_send_data()
 */
typedef void (*bt_socket_data_received_cb)(bt_socket_received_data_s *data, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Called when the socket connection state changes.
 * @param[in] result The result of connection state changing
 * @param[in] connection_state The connection state
 * @param[in] connection The connection information which is established or disconnected
 * @param[in] user_data The user data passed from the callback registration function
 * @pre Either bt_socket_connect_rfcomm() will invoke this function.
 * In addtion, bt_socket_connection_state_changed_cb() will be invoked when the socket connection state is changed.
 * @see bt_socket_listen_and_accept_rfcomm()
 * @see bt_socket_connect_rfcomm()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
typedef void (*bt_socket_connection_state_changed_cb)
	(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *user_data);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 * @brief Initializes the Bluetooth API.
 *
 * @remarks This function must be called before Bluetooth API starts. \n
 * You must free all resources of the Bluetooth service by calling bt_deinitialize() if Bluetooth service is no longer needed.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see  bt_deinitialize()
 */
int bt_initialize(void);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 * @brief Releases all resources of the Bluetooth API.
 *
 * @remarks This function must be called if Bluetooth API is no longer needed.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre Bluetooth API must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 */
int bt_deinitialize(void);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enables the local Bluetooth adapter, asynchronously.
 *
 * @details This function enables Bluetooth protocol stack and hardware.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_ALREADY_DONE  Already enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_initialize()
 * @see bt_adapter_get_state()
 * @see bt_adapter_disable()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 * @see bt_adapter_state_changed_cb()
 *
 */
int bt_adapter_enable(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Disables the local Bluetooth adapter, asynchronously.
 *
 * @details This function disables Bluetooth protocol stack and hardware.
 *
 * @remarks You should disable Bluetooth adapter to switch off Bluetooth chip, which is helpful for saving power.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 *
 * @pre Bluetooth API must be enabled with bt_adapter_enable().
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_adapter_get_state()
 * @see bt_adapter_enable()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb ()
 *
 */
int bt_adapter_disable(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Reset the local Bluetooth adapter, synchronously.
 *
 * @details This function resets Bluetooth protocol and values.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_state_changed_cb() will be invoked if The state of local Bluetooth was #BT_ADAPTER_ENABLED.
 *
 * @see bt_initialize()
 * @see bt_adapter_get_state()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 * @see bt_adapter_state_changed_cb()
 *
 */
int bt_adapter_reset(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the current state of local Bluetooth adapter.
 *
 * @param[out] adapter_state The current adapter state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_enable()
 * @see bt_adapter_disable()
 */
int bt_adapter_get_state(bt_adapter_state_e *adapter_state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the address of local Bluetooth adapter.
 *
 * @remarks The @a local_address must be released with free() by you.
 *
 * @param[out] local_address The device address of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @see bt_adapter_enable()
 * @see bt_adapter_get_name()
 */
int bt_adapter_get_address(char **local_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the name of local Bluetooth adapter.
 *
 * @details Use this function to get the friendly name associated with Bluetooth
 * device, retrieved by the remote Bluetooth devices.
 *
 * @remarks The @a local_name must be released with free() by you.
 *
 * @param[out] local_name  The local device name
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_set_name()
 */
int bt_adapter_get_name(char **local_name);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the name of local Bluetooth adapter.
 *
 * @param[in] local_name The name of the Bluetooth device. \n
 *				The maximum length is 248 characters.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @post bt_adapter_name_changed_cb() will be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_get_name()
 * @see bt_adapter_name_changed_cb()
 * @see bt_adapter_set_name_changed_cb()
 * @see bt_adapter_unset_name_changed_cb()
 */
int bt_adapter_set_name(const char *local_name);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Gets the visibility mode of local Bluetooth adapter.
 * @param[out] mode  The visibility mode of the Bluetooth device
 * @param[out] duration  The duration until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE (in seconds).
 * @a duration is valid only if @a mode is #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE. This value can be NULL.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @see bt_adapter_enable()
 * @see bt_adapter_set_visibility()
 */
int bt_adapter_get_visibility(bt_adapter_visibility_mode_e *mode, int *duration);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the visibility mode.
 *
 * @remarks #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE will change to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE
 * after the given @a duration goes.
 *
 * @param[in] discoverable_mode The Bluetooth visibility mode to set
 * @param[in] duration The duration until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE (in seconds).
 * @a duration is used only for #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE mode.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @post bt_adapter_visibility_mode_changed_cb() will be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_get_visibility()
 * @see bt_adapter_visibility_mode_changed_cb()
 * @see bt_adapter_set_visibility_mode_changed_cb()
 * @see bt_adapter_unset_visibility_mode_changed_cb()
 */
int bt_adapter_set_visibility(bt_adapter_visibility_mode_e discoverable_mode, int duration);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Starts the LE device discovery for a BT_ADAPTER_DEVICE_DISCOVERY_LE type.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 *
 * @details If a LE device is discovered, bt_adapter_le_device_discovery_state_changed_cb()
*  will be invoked with #BT_ADAPTER_LE_DEVICE_DISCOVERY_FOUND, and then bt_adapter_le_device_discovery_state_changed_cb()
 * will be called with #BT_ADAPTER_LE_DEVICE_DISCOVERY_FINISHED in case of the completion or cancellation of the discovery.
 *
 * @remarks To connect to peer Bluetooth device, you need to know its Bluetooth address. \n
 * The device discovery can be stopped by bt_adapter_le_stop_device_discovery().
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation is now in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * or must be #BT_ADAPTER_LE_ENABLED.
 * @post This function invokes bt_adapter_le_device_discovery_state_changed_cb().
 *
 * @see bt_adapter_le_is_discovering()
 * @see bt_adapter_le_device_discovery_state_changed_cb()
 * @see bt_adapter_le_set_device_discovery_state_changed_cb()
 * @see bt_adapter_le_unset_device_discovery_state_changed_cb()
 */
int bt_adapter_le_start_device_discovery(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Starts the device discovery, asynchronously.
 *
 * @details If a device is discovered, bt_adapter_device_discovery_state_changed_cb() will be invoked
 * with #BT_ADAPTER_DEVICE_DISCOVERY_FOUND, and then bt_adapter_device_discovery_state_changed_cb()
 * will be called with #BT_ADAPTER_DEVICE_DISCOVERY_FINISHED in case of the completion or cancellation of the discovery.
 *
 * @remarks To connect to peer Bluetooth device, you need to know its Bluetooth address. \n
 * The device discovery can be stopped by bt_adapter_stop_device_discovery().
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation is now in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @post This function invokes bt_adapter_device_discovery_state_changed_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_is_discovering()
 * @see bt_adapter_stop_device_discovery()
 * @see bt_adapter_device_discovery_state_changed_cb()
 * @see bt_adapter_set_device_discovery_state_changed_cb()
 * @see bt_adapter_unset_device_discovery_state_changed_cb()
 */
int bt_adapter_start_device_discovery(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Stops the device discovery, asynchronously.
 * @remarks The device discovery process will take 10 ~ 20 seconds to get all the devices in vicinity.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation is not in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The device discovery must be in progress with bt_adapter_start_device_discovery().
 * @post This function invokes bt_adapter_device_discovery_state_changed_cb().
 *
 * @see bt_adapter_is_discovering()
 * @see bt_adapter_start_device_discovery()
 * @see bt_adapter_set_device_discovery_state_changed_cb()
 * @see bt_adapter_unset_device_discovery_state_changed_cb()
 * @see bt_adapter_device_discovery_state_changed_cb()
 */
int bt_adapter_stop_device_discovery(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Checks for the device discovery is in progress or not.
 *
 * @remarks If Bluetooth discovery is in progress, other operations are not allowed and
 * you have to either stop the discovery operation, or wait for it to be finished,
 * before performing other operations.

 * @param[out] is_discovering The discovering status: (@c true = in progress , @c  false = not in progress )
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_start_device_discovery()
 * @see bt_adapter_stop_device_discovery()
 */
int bt_adapter_is_discovering(bool *is_discovering);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Retrieves the device information of all bonded devices.
 *
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data passed from the foreach function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @post This function invokes bt_adapter_bonded_device_cb().
 *
 * @see bt_adapter_bonded_device_cb()
 * @see bt_adapter_enable()
 */
int bt_adapter_foreach_bonded_device(bt_adapter_bonded_device_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the device information of a bonded device.
 * @remarks The @a device_info must be released with bt_adapter_free_device_info() by you .
 *
 * @param [in] remote_address The address of remote device
 * @param [out] device_info The bonded device information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @post This function invokes bt_adapter_bonded_device_cb().
 *
 * @see bt_adapter_bonded_device_cb()
 * @see bt_adapter_enable()
 */
int bt_adapter_get_bonded_device_info(const char *remote_address, bt_device_info_s **device_info);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Frees device info.
 *
 * @param [in] device_info The bonded device information
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see bt_adapter_get_bonded_device_info()
 */
int bt_adapter_free_device_info(bt_device_info_s *device_info);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Checks whether the UUID of service is used or not
 * @param[in] service_uuid The UUID of service
 * @param[out] used Indicates whether the service is used or not
 * @return true on success, otherwise false.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see bt_adapter_enable()
 */
int bt_adapter_is_service_used(const char *service_uuid, bool *used);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the Bluetooth adapter state changes.
 *
 * @param[in] callback	The callback function to invoke
 * @param[in] user_data	The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_state_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 */
int bt_adapter_set_state_changed_cb(bt_adapter_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	Unregisters the callback function.
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_set_state_changed_cb()
 */
int bt_adapter_unset_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the name of Bluetooth adapter changes.
 *
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_adapter_name_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_name_changed_cb()
 * @see bt_adapter_unset_name_changed_cb()
 */
int bt_adapter_set_name_changed_cb(bt_adapter_name_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	Unregisters the callback function.
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_set_name_changed_cb()
 */
int bt_adapter_unset_name_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the visibility mode changes.
 *
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_visibility_mode_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_visibility_mode_changed_cb()
 * @see bt_adapter_unset_visibility_mode_changed_cb()
 */
int bt_adapter_set_visibility_mode_changed_cb(bt_adapter_visibility_mode_changed_cb callback, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Unregisters the callback function.
 *
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 *
 * @see  bt_initialize()
 * @see  bt_adapter_set_visibility_mode_changed_cb()
 */
int bt_adapter_unset_visibility_mode_changed_cb(void);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked every second
 * until the visibility mode is changed from #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE
 * to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE.
 * @details  When you set visibility mode as #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE,
 * @a callback will be called every second until visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE.
 * @param[in]  callback  The callback function to register
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre  The Bluetooth service must be initialized by bt_initialize().
 * @post  bt_adapter_visibility_duration_changed_cb() will be invoked.
 * @see  bt_initialize()
 * @see  bt_adapter_set_visibility()
 * @see  bt_adapter_visibility_duration_changed_cb()
 * @see  bt_adapter_unset_visibility_duration_changed_cb()
 */
int bt_adapter_set_visibility_duration_changed_cb(bt_adapter_visibility_duration_changed_cb callback, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	 Unregisters the callback function.
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 * @see  bt_initialize()
 * @see  bt_adapter_set_visibility_duration_changed_cb()
 */
int bt_adapter_unset_visibility_duration_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the device discovery state changes.
 *
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_device_discovery_state_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_device_discovery_state_changed_cb()
 * @see bt_adapter_set_device_discovery_state_changed_cb()
 * @see bt_adapter_unset_device_discovery_state_changed_cb()
 */
int bt_adapter_set_device_discovery_state_changed_cb(bt_adapter_device_discovery_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	Unregisters the callback function.
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_set_device_discovery_state_changed_cb()
 */
int bt_adapter_unset_device_discovery_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the version of local Bluetooth adapter.
 * @since_tizen 2.3
 * @remarks The @a local_version must be released with free() by you.
 *
 * @param[out] local_version The version of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_version(char **local_version);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the information regarding local Bluetooth adapter.
 * @since_tizen 2.3
 * @remarks The @a all parameters must be released with free() by you.
 *
 * @param[out] chipset Chipset name of local Bluetooth adapter
 * @param[out] firmware Firmware info. of local Bluetooth adapter
 * @param[out] stack_version Bluetooth stack version
 * @param[out] profiles The profile list of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_local_info(char **chipset, char **firmware, char **stack_version, char **profiles);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Get the service mask from the uuid list.
 * @since_tizen 2.3
 *
 * @param[in] uuids The UUID list of the device.
 * @param[in] no_of_service The number of the UUID list count.
 * @param[out] service_mask_list Service mask list converted from the given UUID list.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_service_class_t
 */
int bt_device_get_service_mask_from_uuid_list(char **uuids,
					int no_of_service,
					bt_service_class_t *service_mask_list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Recover the local Bluetooth adapter, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function does recovery logic, disables Bluetooth protocol stack and hardware, then enables after a few seconds.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_adapter_get_state()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb ()
 *
 */
int bt_adapter_recover(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Creates a bond with a remote Bluetooth device, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks A bond can be destroyed by bt_device_destroy_bond().\n
 * The bonding request can be cancelled by bt_device_cancel_bonding().
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * with which the bond should be created
 * @param[in] conn_type The connection type(LE or BREDR) to create
 * bond with remote device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY     Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @post This function invokes bt_device_bond_created_cb().
 *
 * @see bt_adapter_start_device_discovery()
 * @see bt_device_create_bond()
 * @see bt_device_bond_created_cb()
 * @see bt_device_cancel_bonding()
 * @see bt_device_destroy_bond()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
int bt_device_create_bond_by_type(const char *remote_address,
				bt_device_connection_link_type_e conn_type);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Creates a bond with a remote Bluetooth device, asynchronously.
 *
 * @remarks A bond can be destroyed by bt_device_destroy_bond().\n
 * The bonding request can be cancelled by bt_device_cancel_bonding().
 *
 * @param[in] remote_address The address of the remote Bluetooth device with which the bond should be created
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable()
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @post This function invokes bt_device_bond_created_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_start_device_discovery()
 * @see bt_device_bond_created_cb()
 * @see bt_device_cancel_bonding()
 * @see bt_device_destroy_bond()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
int bt_device_create_bond(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Cancels the bonding process.
 *
 * @remakrs Use this function when the remote Bluetooth device is not responding to the
 * bond request or you wish to cancel the bonding request.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation not in progress
 * @pre The creating a bond must be in progress by bt_device_create_bond().
 *
 * @see bt_device_create_bond()
 * @see bt_device_bond_created_cb()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
int bt_device_cancel_bonding(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Destroys the bond, asynchronously.
 *
 * @param[in] remote_address The address of the remote Bluetooth device to remove bonding
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @pre The bond with the remote device must be created with bt_device_create_bond().
 * @post This function invokes bt_device_bond_destroyed_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_device_create_bond()
 * @see bt_device_bond_destroyed_cb()
 * @see bt_device_set_bond_destroyed_cb()
 * @see bt_device_unset_bond_destroyed_cb()
 */
int bt_device_destroy_bond(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Sets an alias for the bonded device.
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[in] alias The alias of the remote Bluetooth device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @pre The bond with the remote device must be created with bt_device_create_bond().
 *
 * @see bt_adapter_enable()
 * @see bt_device_create_bond()
 */
int bt_device_set_alias(const char *remote_address, const char *alias);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Sets the authorization of a bonded device, asynchronously.
 *
 * @remarks Once a device is authorized, you don't need to receive a confirmation.
 *
 * @param[in] remote_address The address of the remote Bluetooth device to authorize
 * @param[in] authorization The Bluetooth authorization state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @pre The bond with the remote device must be created with bt_device_create_bond().
 * @post bt_device_authorization_changed_cb() will be invoked.
 *
 * @see bt_adapter_enable()
 * @see bt_device_create_bond()
 * @see bt_device_authorization_changed_cb()
 * @see bt_device_set_authorization_changed_cb()
 * @see bt_device_unset_authorization_changed_cb()
 */
int bt_device_set_authorization(const char *remote_address, bt_device_authorization_e authorization_state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Starts the search for services supported by the specified device, asynchronously.
 *
 * @remarks If creating a bond succeeds, which means bt_device_bond_created_cb() is called with result #BT_ERROR_NONE,
 * then you don't need to run this function.\n
 * The service search takes a couple of seconds to complete normally. \n
 * The service search can be canceled by bt_device_cancel_service_search().
 *
 * @param[in] remote_address The address of the remote Bluetooth device whose services need to be checked
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_SERVICE_SEARCH_FAILED  Service search failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @pre The bond with the remote device must be created with bt_device_create_bond().
 * @post This function invokes bt_device_service_searched_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_start_device_discovery()
 * @see bt_device_create_bond()
 * @see bt_device_bond_created_cb()
 * @see bt_device_service_searched_cb()
 * @see bt_device_cancel_service_search()
 * @see bt_device_set_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 */
int bt_device_start_service_search(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Cancels service search process.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation not in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The service search must be in progress by bt_device_start_service_search().
 *
 * @see bt_device_start_service_search()
 * @see bt_device_service_searched_cb()
 * @see bt_device_set_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 */
int bt_device_cancel_service_search(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets the connected profiles.
 * @param[in] remote_address The address of the remote device
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @post bt_device_connected_profile() will be invoked.
 * @see bt_adapter_enable()
 * @see bt_device_connected_profile()
 */
int bt_device_foreach_connected_profiles(const char *remote_address, bt_device_connected_profile callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets the profile connected status.
 * @since_tizen 2.3
 * @param[in] remote_address The address of the remote device
 * @param[in] bt_profile wish to know bt_profile
 * @param[out] connected_status the connected status
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED   Remote device not bonded
 * @retval #BT_ERROR_OPERATION_FAILED   Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_device_is_profile_connected(const char *remote_address, bt_profile_e bt_profile,
					bool *connected_status);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Registers a callback function to be invoked when the bond creates.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_device_bond_created_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_device_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
int bt_device_set_bond_created_cb(bt_device_bond_created_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief	Unregisters the callback function.
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_device_set_bond_created_cb()
 */
int bt_device_unset_bond_created_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Registers a callback function to be invoked when the bond destroys.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_device_bond_destroyed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_device_bond_destroyed_cb()
 * @see bt_device_unset_bond_destroyed_cb()
 */
int bt_device_set_bond_destroyed_cb(bt_device_bond_destroyed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief	Unregisters the callback function.
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_device_set_bond_destroyed_cb()
 */
int bt_device_unset_bond_destroyed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Registers a callback function to be invoked when the authorization of device changes.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_device_authorization_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_device_authorization_changed_cb()
 * @see bt_device_set_authorization_changed_cb()
 * @see bt_device_unset_authorization_changed_cb()
 */
int bt_device_set_authorization_changed_cb(bt_device_authorization_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief	Unregisters the callback function.
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_device_set_authorization_changed_cb()
 */
int bt_device_unset_authorization_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Registers a callback function to be invoked when the process of service search finishes.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_device_service_searched_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_device_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 */
int bt_device_set_service_searched_cb(bt_device_service_searched_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief	Unregisters the callback function.
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_device_set_service_searched_cb()
 */
int bt_device_unset_service_searched_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Registers a callback function to be invoked when the connection state is changed.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_device_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_device_connection_state_changed_cb()
 * @see bt_device_unset_connection_state_changed_cb()
 */
int bt_device_set_connection_state_changed_cb(bt_device_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief	Unregisters the callback function to be invoked when the connection state is changed.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_device_set_connection_state_changed_cb()
 */
int bt_device_unset_connection_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Registers a rfcomm socket with a specific UUID.
 *
 * @remarks A socket can be destroyed by bt_socket_destroy_rfcomm().
 *
 * @param[in] service_uuid The UUID of service to provide
 * @param[out] socket_fd The file descriptor of socket to listen
 * @return 0 on success, otherwise a negative error value.
 *
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 *
 * @see bt_adapter_enable()
 * @see bt_socket_listen_and_accept_rfcomm()
 * @see bt_socket_destroy_rfcomm()
 */
int bt_socket_create_rfcomm(const char *service_uuid, int *socket_fd);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Removes the rfcomm socket with the given socket.
 * @remarks If callback function bt_socket_connection_state_changed_cb() is set and the remote Bluetooth device is connected,
 * then bt_socket_connection_state_changed_cb() will be called when this function is finished successfully.
 *
 * @param[in] socket_fd The file descriptor of socket to destroy
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The socket must be created with bt_socket_create_rfcomm().
 * @post If callback function bt_socket_connection_state_changed_cb() is set and the remote Bluetooth device is connected,
 * then bt_socket_connection_state_changed_cb() will be called.
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_destroy_rfcomm(int socket_fd);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Starts listening on passed rfcomm socket and accepts connection requests.
 * @details Pop-up is shown automatically when a RFCOMM connection is requested.
 * bt_socket_connection_state_changed_cb() will be called with
 * #BT_SOCKET_CONNECTED if you click "yes" and connection is finished successfully.
 * @param[in] socket_fd The file descriptor of socket on which start to listen
 * @param[in] max_pending_connections The maximum number of pending connections
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The socket must be created with bt_socket_create_rfcomm().
 * @post If callback function bt_socket_connection_state_changed_cb() is set,
 * then bt_socket_connection_state_changed_cb() will be called when the remote Bluetooth device is connected.
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_listen_and_accept_rfcomm(int socket_fd, int max_pending_connections);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Called when a RFCOMM connection is requested.
 * @details You must call bt_socket_accept() if you want to accept. Otherwise, you must call bt_socket_reject().
 * @param[in] socket_fd  The file descriptor of socket on which a connection is requested
 * @param[in] remote_address  The address of remote device
 * @param[in] user_data The user data passed from the callback registration function
 * @pre If you register this callback function by bt_socket_set_connection_requested_cb() and listen a socket by bt_socket_listen(),
 * bt_socket_connection_requested_cb() will be invoked.
 * @see bt_socket_listen()
 * @see bt_socket_accept()
 * @see bt_socket_reject()
 */
typedef void (*bt_socket_connection_requested_cb) (int socket_fd, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Starts listening on passed rfcomm socket.
 * @details bt_socket_connection_requested_cb() will be called when a RFCOMM connection is requested.
 *
 * @param[in] socket_fd  The file descriptor socket on which start to listen
 * @param[in] user_data  The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The socket must be created with bt_socket_create_rfcomm().
 * @post This function invokes bt_socket_connection_state_changed_cb().
 *
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_set_connection_requested_cb()
 * @see bt_socket_unset_connection_requested_cb()
 * @see bt_socket_connection_requested_cb()
 */
int bt_socket_listen(int socket_fd, int max_pending_connections);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Accepts a connection request.
 * @param[in] requested_socket_fd  The file descriptor of socket on which a connection is requested
 * @param[out] connected_socket_fd  The file descriptor of connected socket
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The connection is requested by bt_socket_connection_requested_cb().
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_requested_cb()
 * @see bt_socket_listen()
 * @see bt_socket_reject()
*/
int bt_socket_accept(int requested_socket_fd);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Rejects a connection request.
 * @param[in] socket_fd  The file descriptor of socket on which a connection is requested
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The connection is requested by bt_socket_connection_requested_cb().
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_requested_cb()
 * @see bt_socket_listen()
 * @see bt_socket_accept()
 */
int bt_socket_reject(int socket_fd);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Connects to a specific RFCOMM based service on a remote Bluetooth device UUID, asynchronously.
 *
 * @remarks A connection can be disconnected by bt_socket_disconnect_rfcomm().
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[in] service_uuid The UUID of service provided by the remote Bluetooth device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable().
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @pre The bond with the remote device must be created with bt_device_create_bond().
 * @post This function invokes bt_socket_connection_state_changed_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_device_create_bond()
 * @see bt_adapter_start_device_discovery()
 * @see bt_device_start_service_search()
 * @see bt_socket_disconnect_rfcomm()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_connect_rfcomm(const char *remote_address, const char *service_uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Disconnects the RFCOMM connection with the given file descriptor of conneted socket.
 * @remarks Because this function is synchronous, bt_socket_connection_state_changed_cb() won't be called.
 * @param[in] socket_fd  The file descriptor of socket to close
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The connection must be established.
 */
int bt_socket_disconnect_rfcomm(int socket_fd);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Sends data to the connected device.
 *
 * @param[in] socket_fd The file descriptor of connected socket
 * @param[in] data The data to be sent
 * @param[in] length The length of data to be sent
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The connection must be established.
 */
int bt_socket_send_data(int socket_fd, const char *data, int length);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Register a callback function that will be invoked when you receive data.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_socket_data_received_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_socket_data_received_cb()
 * @see bt_socket_set_data_received_cb()
 * @see bt_socket_unset_data_received_cb()
 */
int bt_socket_set_data_received_cb(bt_socket_data_received_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief	Unregisters the callback function.
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_socket_data_received_cb()
 * @see bt_socket_set_data_received_cb()
 */
int bt_socket_unset_data_received_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Register a callback function that will be invoked when a RFCOMM connection is requested.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post If you listen a socket by bt_socket_listen(), bt_socket_connection_requested_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_socket_unset_connection_requested_cb()
 */
int bt_socket_set_connection_requested_cb(bt_socket_connection_requested_cb callback, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Unregisters the callback function.
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 * @see  bt_initialize()
 * @see  bt_socket_set_connection_requested_cb()
 * @see  bt_socket_connection_requested_cb()
 */
int bt_socket_unset_connection_requested_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Register a callback function that will be invoked when the connection state changes.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_socket_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_set_connection_state_changed_cb(bt_socket_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief	Unregisters the callback function.
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 */
int bt_socket_unset_connection_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when the push is requested.
 * @details You must call bt_opp_server_accept() if you want to accept.
 * Otherwise, you must call bt_opp_server_reject().
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_initialize()
 */
typedef void (*bt_opp_server_push_requested_cb)(const char *file, int size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when an OPP connection is requested.
 * @details You must call bt_opp_server_accept_connection() if you want to accept.
 * Otherwise, you must call bt_opp_server_reject_connection().
 * @param[in] remote_address  The address of remote device
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_initialize()
 * @see bt_opp_server_accept_connection()
 * @see bt_opp_server_reject_connection()
 */
typedef void (*bt_opp_server_connection_requested_cb)(const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Initializes the Bluetooth OPP server requested by bt_opp_server_push_requested_cb().
 * @details The popup appears when an OPP connection is requested from a remote device.
 * If you accept the request, then connection will be established and bt_opp_server_push_requested_cb() will be called.
 * At that time, you can call either bt_opp_server_accept() or bt_opp_server_reject().
 * @remarks This function must be called to start Bluetooth OPP server. You must free all resources of the Bluetooth service
 * by calling bt_opp_server_deinitialize() if Bluetooth OPP service is no longer needed.
 * @param[in] destination  The destination path
 * @param[in] push_requested_cb  The callback called when a push is requested
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see  bt_opp_server_push_requested_cb()
 * @see  bt_opp_server_deinitialize()
 * @see  bt_opp_server_accept()
 * @see  bt_opp_server_reject()
 */
int bt_opp_server_initialize(const char *destination, bt_opp_server_push_requested_cb push_requested_cb, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Initializes the Bluetooth OPP server requested by bt_opp_server_connection_requested_cb().
 * @details No popup appears when an OPP connection is requested from a remote device.
 * Instead, @a connection_requested_cb() will be called.
 * At that time, you can call either bt_opp_server_accept() or bt_opp_server_reject().
 * @remarks This function must be called to start Bluetooth OPP server. \n
 * You must free all resources of the Bluetooth service by calling bt_opp_server_deinitialize() if Bluetooth OPP service is no longer needed.
 * @param[in] destination  The destination path
 * @param[in] connection_requested_cb  The callback called when an OPP connection is requested
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see  bt_opp_server_connection_requested_cb()
 * @see  bt_opp_server_deinitialize()
 * @see  bt_opp_server_accept_connection()
 * @see  bt_opp_server_reject_connection()
 */
int bt_opp_server_initialize_by_connection_request(const char *destination, bt_opp_server_connection_requested_cb connection_requested_cb, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Denitializes the Bluetooth OPP server.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see  bt_opp_server_initialize()
 * @see  bt_opp_server_deinitialize()
 * @see  bt_opp_server_initialize()
 */
int bt_opp_server_deinitialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when a file is being transfered.
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] percent The progress in percentage (1 ~ 100)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_accept()
 * @see bt_opp_server_accept_connection()
 */
typedef void (*bt_opp_server_transfer_progress_cb) (const char *file, long long size, int percent, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when a transfer is finished.
 * @param[in] error_code  The result of push
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_accept()
 * @see bt_opp_server_accept_connection()
 */
typedef void (*bt_opp_server_transfer_finished_cb) (int result, const char *file, long long size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Accepts the push request from the remote device.
 * @remarks If you initialize OPP server by bt_opp_server_initialize_by_connection_request(), then name is ignored.
 * You can cancel the pushes by bt_opp_server_cancel_transfer() with transfer_id.
 * @param[in] progress_cb  The callback called when a file is being transfered
 * @param[in] finished_cb  The callback called when a transfer is finished
 * @param[in] name  The name to store. This can be NULL if you initialize OPP server by bt_opp_server_initialize_by_connection_request().
 * @param[in] user_data The user data to be passed to the callback function
 * @param[out]  transfer_id  The ID of transfer
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @see  bt_opp_server_reject()
 */
int bt_opp_server_accept(bt_opp_server_transfer_progress_cb progress_cb, bt_opp_server_transfer_finished_cb finished_cb, const char *name,
 void *user_data, int *transfer_id);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Rejects the push request from the remote device.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see  bt_opp_server_accept()
 */
int bt_opp_server_reject(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Cancels the transfer.
 * @param[in] transfer_id  The ID of transfer
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see  bt_opp_server_accept_connection()
 * @see  bt_opp_server_accept()
 */
int bt_opp_server_cancel_transfer(int transfer_id);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Sets the destination path of file to be pushed.
 * @param[in] destination  The destination path of file to be pushed
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see  bt_opp_server_initialize()
 */
int bt_opp_server_set_destination(const char *destination);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief Initializes the Bluetooth OPP client.
 ** @remarks This function must be called before Bluetooth OPP client starts. \n
 ** You must free all resources of the Bluetooth service by calling bt_opp_client_deinitialize()
 ** if Bluetooth OPP service is no longer needed.
 ** @return 0 on success, otherwise a negative error value.
 ** @retval #BT_ERROR_NONE  Successful
 ** @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 ** @retval #BT_ERROR_NOT_ENABLED  Not enabled
 ** @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 ** @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 ** @see  bt_opp_client_deinitialize()
 **/
int bt_opp_client_initialize(void);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief Denitializes the Bluetooth OPP client.
 ** @return 0 on success, otherwise a negative error value.
 ** @retval #BT_ERROR_NONE  Successful
 ** @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 ** @retval #BT_ERROR_NOT_ENABLED  Not enabled
 ** @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 ** @see  bt_opp_client_initialize()
 **/
int bt_opp_client_deinitialize(void);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief Adds file to be pushed.
 ** @param[in] file  The path of file to be pushed
 ** @return 0 on success, otherwise a negative error value.
 ** @retval #BT_ERROR_NONE  Successful
 ** @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 ** @retval #BT_ERROR_NOT_ENABLED  Not enabled
 ** @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 ** @see  bt_opp_client_clear_files()
 ** @see  bt_opp_client_push_files()
 **/
int bt_opp_client_add_file(const char *file);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief Adds file to be pushed.
 ** @return 0 on success, otherwise a negative error value.
 ** @retval #BT_ERROR_NONE  Successful
 ** @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 ** @retval #BT_ERROR_NOT_ENABLED  Not enabled
 ** @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 ** @see  bt_opp_client_add_file()
 ** @see  bt_opp_client_push_files()
 **/
int bt_opp_client_clear_files(void);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief  Called when OPP server responds to the push request.
 ** @param[in] result  The result of OPP server response
 ** @param[in] remote_address  The remote address
 ** @param[in] user_data  The user data passed from the callback registration function
 ** @pre bt_opp_client_push_files() will invoke this function.
 ** @see bt_opp_client_push_files()
 **/
typedef void (*bt_opp_client_push_responded_cb)(int result, const char *remote_address, void *user_data);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief  Called when each file is being transfered.
 ** @param[in] file  The path of file to be pushed
 ** @param[in] size The file size (bytes)
 ** @param[in] percent The progress in percentage (1 ~ 100). 100 means that a file is transfered completely.
 ** @param[in] user_data The user data passed from the callback registration function
 ** @pre bt_opp_client_push_files() will invoke this function.
 ** @see bt_opp_client_push_files()
 **/
typedef void (*bt_opp_client_push_progress_cb)(const char *file, long long size, int percent, void *user_data);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief  Called when the push request is finished.
 ** @param[in] result  The result of the push request
 ** @param[in] remote_address  The remote address
 ** @param[in] user_data The user data passed from the callback registration function
 ** @see bt_opp_client_push_files()
 **/
typedef void (*bt_opp_client_push_finished_cb)(int result, const char *remote_address, void *user_data);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief Pushes the file to the remote device, asynchronously.
 ** @details At first, bt_opp_client_push_responded_cb() will be called when OPP server responds to the push request.
 ** After connection is established, bt_opp_client_push_progress_cb() will be called repeatedly until a file is tranfered completely.
 ** If you send several files, then bt_opp_client_push_progress_cb() with another file will be called repeatedly until the file is tranfered completely.
 ** bt_opp_client_push_finished_cb() will be called when the push request is finished.
 ** @param[in] remote_address The remote address
 ** @param[in] responded_cb  The callback called when OPP server responds to the push request
 ** @param[in] progress_cb  The callback called when each file is being transfered
 ** @param[in] finished_cb  The callback called when the push request is finished
 ** @param[in] user_data The user data to be passed to the callback function
 ** @return 0 on success, otherwise a negative error value.
 ** @retval #BT_ERROR_NONE  Successful
 ** @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 ** @retval #BT_ERROR_NOT_ENABLED  Not enabled
 ** @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 ** @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 ** @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 ** @see bt_opp_client_initialize()
 ** @see bt_opp_client_cancel_push
 **/
int bt_opp_client_push_files(const char *remote_address, bt_opp_client_push_responded_cb responded_cb,
 bt_opp_client_push_progress_cb progress_cb, bt_opp_client_push_finished_cb finished_cb, void *user_data);

/**
 ** @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 ** @brief Cancels the push request in progress, asynchronously.
 ** @return 0 on success, otherwise a negative error value.
 ** @retval #BT_ERROR_NONE  Successful
 ** @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 ** @retval #BT_ERROR_NOT_ENABLED  Not enabled
 ** @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 ** @pre bt_opp_client_push_files() must be called.
 ** @post bt_opp_client_push_finished_cb() will be invoked with result #BT_ERROR_CANCELLED,
 ** which is a parameter of bt_opp_client_push_files().
 ** @see bt_opp_client_initialize()
 ** @see bt_opp_client_push_files()
 **/
int bt_opp_client_cancel_push(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Called when the connection state is changed.
 * @param[in] connected  Indicates whether a client is connected or disconnected
 * @param[in] remote_address  The remote address
 * @param[in] interface_name  The interface name. For example, bnep0, bnep1.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_nap_set_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
typedef void (*bt_nap_connection_state_changed_cb) (bool connected, const char *remote_address, const char *interface_name, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Activates the NAP(Network Access Point).
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @pre The Bluetooth must be enabled with bt_adapter_enable().
 * @see bt_adapter_enable()
 * @see bt_nap_deactivate()
 */
int bt_nap_activate(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Deactivates the NAP(Network Access Point).
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_deactivate(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Disconnects the all PANUs(Personal Area Networking User) which are connected to you.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_disconnect_all(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Disconnects the specified PANU(Personal Area Networking User) which is connected to you.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_disconnect(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state changes.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_nap_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
int bt_nap_set_connection_state_changed_cb(bt_nap_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state changes.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_nap_connection_state_changed_cb()
 * @see bt_nap_set_connection_state_changed_cb()
 */
int bt_nap_unset_connection_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Called when the connection state is changed.
 * @details  This callback is called when the connection state is changed.
 * When you call bt_panu_connect() or bt_panu_disconnect(), this callback is also called with error result even though these functions fail.
 * @param[in] result  The result of changing the connection state
 * @param[in] connected  The state to be changed. @a true means connected state, Otherwise, @a false.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of PAN service
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_nap_set_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
typedef void (*bt_panu_connection_state_changed_cb) (int result, bool connected, const char *remote_address, bt_panu_service_type_e type, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state changes.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_panu_connection_state_changed_cb()
 * @see bt_panu_unset_connection_state_changed_cb()
 */
int bt_panu_set_connection_state_changed_cb(bt_panu_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state changes.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_panu_connection_state_changed_cb()
 * @see bt_panu_set_connection_state_changed_cb()
 */
int bt_panu_unset_connection_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief Connects the remote device with the PAN(Personal Area Networking) service, asynchronously.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of PAN service
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @post bt_panu_connection_state_changed_cb() will be invoked.
 * @see bt_panu_disconnect()
 * @see bt_panu_connection_state_changed_cb()
 */
int bt_panu_connect(const char *remote_address, bt_panu_service_type_e type);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief Disconnects the remote device with the PAN(Personal Area Networking) service, asynchronously.
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The remote device must be connected by bt_panu_connect().
 * @post bt_panu_connection_state_changed_cb() will be invoked.
 * @see bt_panu_connect()
 * @see bt_panu_connection_state_changed_cb()
 */
int bt_panu_disconnect(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief  Called when the connection state is changed.
 * @details  This callback is called when the connection state is changed.
 * When you call bt_hid_host_connect() or bt_hid_host_disconnect(), this callback is also called with error result even though these functions fail.
 * @param[in] result  The result of changing the connection state
 * @param[in] connected  The state to be changed. @a true means connected state, Otherwise, @a false.
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hid_host_connect()
 * @see bt_hid_host_disconnect()
 */
typedef void (*bt_hid_host_connection_state_changed_cb) (int result, bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Initializes the Bluetooth HID(Human Interface Device) Host.
 * @remarks This function must be called before Bluetooth HID Host starts. \n
 * You must free all resources of the Bluetooth service by calling bt_hid_host_deinitialize()
 * if Bluetooth HID Host service is no longer needed.
 * @param[in] connection_cb  The callback called when the connection state is changed
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see  bt_hid_host_deinitialize()
 */
int bt_hid_host_initialize(bt_hid_host_connection_state_changed_cb connection_cb, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Deinitializes the Bluetooth HID(Human Interface Device) Host.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth HID service must be initialized with bt_hid_host_initialize().
 * @see  bt_hid_host_initialize()
 */
int bt_hid_host_deinitialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Connects the remote device with the HID(Human Interface Device) service, asynchronously.
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @pre The Bluetooth HID service must be initialized with bt_hid_host_initialize().
 * @post bt_hid_host_connection_state_changed_cb() will be invoked.
 * @see bt_hid_host_disconnect()
 * @see bt_hid_host_connection_state_changed_cb()
 */
int bt_hid_host_connect(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Disconnects the remote device with the HID(Human Interface Device) service, asynchronously.
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The remote device must be connected by bt_hid_host_connect().
 * @post bt_hid_host_connection_state_changed_cb() will be invoked.
 * @see bt_hid_host_connect()
 * @see bt_hid_host_connection_state_changed_cb()
 */
int bt_hid_host_disconnect(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief Initializes the Bluetooth profiles related with audio.
 * @remarks This function must be called before Bluetooth profiles related with audio starts. \n
 * You must free all resources of the this service by calling bt_audio_deinitialize()
 * if Bluetooth profiles related with audio service is no longer needed.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_audio_deinitialize()
 */
int bt_audio_initialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief Deinitializes the Bluetooth profiles related with audio.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 */
int bt_audio_deinitialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief Connects the remote device with the given audio profile, asynchronously.
 * @details If you input type as #BT_AUDIO_PROFILE_TYPE_ALL and connection request succeeds, then bt_audio_connection_state_changed_cb() will be called twice
 * when #BT_AUDIO_PROFILE_TYPE_HSP_HFP is connected and #BT_AUDIO_PROFILE_TYPE_A2DP is connected.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of audio profile
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @post bt_audio_connection_state_changed_cb() will be invoked.
 * @see bt_audio_disconnect()
 * @see bt_audio_connection_state_changed_cb()
 */
int bt_audio_connect(const char *remote_address, bt_audio_profile_type_e type);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief Disconnects the remote device with the given audio profile, asynchronously.
 * @details If you input type as #BT_AUDIO_PROFILE_TYPE_ALL and disconnection request succeeds, then bt_audio_connection_state_changed_cb() will be called twice
 * when #BT_AUDIO_PROFILE_TYPE_HSP_HFP is disconnected and #BT_AUDIO_PROFILE_TYPE_A2DP is disconnected.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of audio profile
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The remote device must be connected by bt_audio_connect().
 * @post bt_audio_connection_state_changed_cb() will be invoked.
 * @see bt_audio_connect()
 * @see bt_audio_connection_state_changed_cb()
 */
int bt_audio_disconnect(const char *remote_address, bt_audio_profile_type_e type);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Called when the connection state is changed.
 * @details  This callback is called when the connection state is changed.
 * When you call bt_audio_connect() or bt_audio_disconnect(), this callback is also called with error result even though these functions fail.
 * @param[in] result  The result of changing the connection state
 * @param[in] connected  The state to be changed. @a true means connected state, Otherwise, @a false.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of audio profile except #BT_AUDIO_PROFILE_TYPE_ALL
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_audio_set_connection_state_changed_cb()
 * @see bt_audio_unset_connection_state_changed_cb()
 */
typedef void (*bt_audio_connection_state_changed_cb) (int result, bool connected, const char *remote_address, bt_audio_profile_type_e type, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state is changed.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_audio_connection_state_changed_cb()
 * @see bt_panu_unset_connection_state_changed_cb()
 */
int bt_audio_set_connection_state_changed_cb(bt_audio_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state is changed.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_audio_connection_state_changed_cb()
 * @see bt_audio_set_connection_state_changed_cb()
 */
int bt_audio_unset_connection_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Opens a SCO(Synchronous Connection Oriented link) to connected remote device, asynchronously.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @post bt_ag_sco_state_changed_cb() will be invoked.
 * @see bt_ag_close_sco()
 * @see bt_ag_sco_state_changed_cb()
 * @see bt_audio_connect()
 */
int bt_ag_open_sco(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Closes an opened SCO(Synchronous Connection Oriented link), asynchronously.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The SCO must be opened with bt_ag_open_sco().
 * @post bt_ag_sco_state_changed_cb() will be invoked.
 * @see bt_ag_open_sco()
 * @see bt_ag_sco_state_changed_cb()
 */
int bt_ag_close_sco(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Checks whether an opened SCO(Synchronous Connection Oriented link) exists or not.
 * @param[out] opened The SCO status: (@c true = opened, @c  false = not opened)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_adapter_enable()
 * @see bt_ag_open_sco()
 * @see bt_ag_close_sco()
 */
int bt_ag_is_sco_opened(bool *opened);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the SCO(Synchronous Connection Oriented link) state is changed.
 * @details  This callback is called when the SCO state is changed.
 * When you call bt_ag_open_sco() or bt_ag_close_sco(), this callback is also called with error result even though these functions failed.
 * @param[in] result  The result of changing the connection state
 * @param[in] opened  The state to be changed: (@c true = opened, @c  false = not opened)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 * @see bt_ag_open_sco()
 * @see bt_ag_close_sco()
 */
typedef void (*bt_ag_sco_state_changed_cb) (int result, bool opened, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when the SCO(Synchronous Connection Oriented link) state is changed.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 */
int bt_ag_set_sco_state_changed_cb(bt_ag_sco_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when the SCO(Synchronous Connection Oriented link) state is changed.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_sco_state_changed_cb()
 * @see bt_ag_set_sco_state_changed_cb()
 */
int bt_ag_unset_sco_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the call event to the remote bluetooth device.
 * @remarks Before notifying #BT_AG_CALL_EVENT_ANSWERED or #BT_AG_CALL_EVENT_DIALING, you should open SCO(Synchronous Connection Oriented link)
 * if Bluetooth Hands-Free need SCO connection.
 * @param[in] event  The call event
 * @param[in] call_id  The call ID
 * @param[in] phone_number  The phone number. You must set this value in case of #BT_AG_CALL_EVENT_DIALING and #BT_AG_CALL_EVENT_INCOMING.
 * In other cases, this value can be NULL.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_call_event(bt_ag_call_event_e event, unsigned int call_id, const char *phone_number);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the call list to the remote bluetooth device.
 * @param[in] list  The call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_call_list(bt_call_list_h list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the state of voice recognition.
 * @param[in] state  The state of voice recognition: (@c true = enabled, @c  false = disabled)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_voice_recognition_state(bool state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a call handling event happened from Hands-Free.
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] call_id  The call ID
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
typedef void (*bt_ag_call_handling_event_cb) (bt_ag_call_handling_event_e event, unsigned int call_id, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
int bt_ag_set_call_handling_event_cb(bt_ag_call_handling_event_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_set_call_handling_event_cb()
 */
int bt_ag_unset_call_handling_event_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a multi call handling event happened from Hands-Free.
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
typedef void (*bt_ag_multi_call_handling_event_cb) (bt_ag_multi_call_handling_event_e event, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
int bt_ag_set_multi_call_handling_event_cb(bt_ag_multi_call_handling_event_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_set_multi_call_handling_event_cb()
 */
int bt_ag_unset_multi_call_handling_event_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @param[in] dtmf  The DTMF transmitted from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_dtmf_transmitted_cb()
 * @see bt_ag_unset_dtmf_transmitted_cb()
 */
typedef void (*bt_ag_dtmf_transmitted_cb) (const char *dtmf, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_dtmf_transmitted_cb()
 * @see bt_ag_unset_dtmf_transmitted_cb()
 */
int bt_ag_set_dtmf_transmitted_cb(bt_ag_dtmf_transmitted_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_dtmf_transmitted_cb()
 * @see bt_ag_set_dtmf_transmitted_cb()
 */
int bt_ag_unset_dtmf_transmitted_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Notifies the speaker gain to the remote device.
 * @details This function sends a signal to the remote device. This signal has the gain value.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * When the speaker gain of remote device is changed to the requested gain, bt_audio_speaker_gain_changed_cb() will be called.
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_ag_get_speaker_gain()
 * @see bt_ag_set_speaker_gain_changed_cb()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
int bt_ag_notify_speaker_gain(int gain);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Gets the current speaker gain of the remote device.
 * @details This function gets the value of speaker gain of the remote device.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * @param[out] gain The gain of speaker (0 ~ 15)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_ag_notify_speaker_gain()
 * @see bt_ag_set_speaker_gain_changed_cb()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
int bt_ag_get_speaker_gain(int *gain);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Checks whether the remoted device enables NREC(Noise Reduction and Echo Canceling) or not.
 * @param[out] enabled The NREC status: (@c true = enabled, @c  false = not enabled)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_audio_connect()
 */
int bt_ag_is_nrec_enabled(bool *enabled);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the speaker gain of the remote device is changed.
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_speaker_gain_changed_cb()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
typedef void (*bt_ag_speaker_gain_changed_cb) (int gain, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when the speaker gain of the remote device is changed.
 * @details This function let you know the change of the speaker gain of the remote device.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
int bt_ag_set_speaker_gain_changed_cb(bt_ag_speaker_gain_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when the speaker gain of the remote device is changed.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_set_speaker_gain_changed_cb()
 */
int bt_ag_unset_speaker_gain_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the microphone gain of the remote device is changed.
 * @param[in] gain The gain of microphone (0 ~ 15)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_microphone_gain_changed_cb()
 * @see bt_ag_unset_microphone_gain_changed_cb()
 */
typedef void (*bt_ag_microphone_gain_changed_cb) (int gain, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when the microphone gain of the remote device is changed.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_unset_microphone_gain_changed_cb()
 */
int bt_ag_set_microphone_gain_changed_cb(bt_ag_microphone_gain_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when the microphone gain of the remote device is changed.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_set_microphone_gain_changed_cb()
 */
int bt_ag_unset_microphone_gain_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Creates a handle of call list.
 * @param[out] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @see bt_call_list_destroy()
 */
int bt_call_list_create(bt_call_list_h *list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Destroys the handle of call list.
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @see bt_call_list_create()
 */
int bt_call_list_destroy(bt_call_list_h list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Resets the handle of call list.
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @see bt_call_list_create()
 */
int bt_call_list_reset(bt_call_list_h list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Adds a call to the handle of call list.
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @see bt_call_list_create()
 */
int bt_call_list_add(bt_call_list_h list, unsigned int call_id, bt_ag_call_state_e state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the connection state is changed.
 * @param[in] connected  The state to be changed. @a true means connected state, Otherwise, @a false.
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_target_initialize()
 * @see bt_avrcp_target_deinitialize()
 */
typedef void (*bt_avrcp_target_connection_state_changed_cb) (bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Initializes the Bluetooth AVRCP(Audio/Video Remote Control Profile) service.
 * @remarks This function must be called before Bluetooth AVRCP service. \n
 * You must free all resources of the this service by calling bt_avrcp_target_deinitialize()
 * if Bluetooth AVRCP service is no longer needed.
 * @param[in] callback The callback function called when the connection state is changed
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_target_deinitialize()
 */
int bt_avrcp_target_initialize(bt_avrcp_target_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Deinitializes the Bluetooth AVRCP(Audio/Video Remote Control Profile) service.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth audio service must be initialized with bt_avrcp_target_initialize().
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_deinitialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the equalize state to the remote device.
 * @param[in] state The state of equalizer
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_equalizer_state(bt_avrcp_equalizer_state_e state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the repeat mode to the remote device.
 * @param[in] mode The repeat mode
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_repeat_mode(bt_avrcp_repeat_mode_e mode);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the shuffle mode to the remote device.
 * @param[in] mode The repeat mode
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_shuffle_mode(bt_avrcp_shuffle_mode_e mode);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the scan mode to the remote device.
 * @param[in] mode The scan mode
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_scan_mode(bt_avrcp_scan_mode_e mode);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the player state to the remote device.
 * @param[in] state The player state
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_player_state(bt_avrcp_player_state_e state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the current position of song to the remote device.
 * @param[in] position The current position in milliseconds
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_position(unsigned int position);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Notifies the track to the remote device.
 * @param[in] title The title of track
 * @param[in] artist The artist of track
 * @param[in] album The album of track
 * @param[in] genre The genre of track
 * @param[in] track_num The track number
 * @param[in] total_tracks The number of all tracks
 * @param[in] duration The duration of track in milliseconds
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_target_notify_track(const char *title, const char *artist, const char *album, const char *genre, unsigned int track_num, unsigned int total_tracks, unsigned int duration);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the equalizer state is changed by the remote control device.
 * @param[in] equalizer The equalizer state
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_equalizer_state_changed_cb()
 * @see bt_avrcp_unset_equalizer_state_changed_cb()
 */
typedef void (*bt_avrcp_equalizer_state_changed_cb) (bt_avrcp_equalizer_state_e equalizer, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the equalizer state is changed by the remote control device.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_equalizer_state_changed_cb()
 */
int bt_avrcp_set_equalizer_state_changed_cb(bt_avrcp_equalizer_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the equalizer state is changed by the remote control device.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_set_equalizer_state_changed_cb()
 */
int bt_avrcp_unset_equalizer_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the repeat mode is changed by the remote control device.
 * @param[in] repeat The repeat mode
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_repeat_mode_changed_cb()
 * @see bt_avrcp_unset_repeat_mode_changed_cb()
 */
typedef void (*bt_avrcp_repeat_mode_changed_cb) (bt_avrcp_repeat_mode_e repeat, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the repeat mode is changed by the remote control device.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_repeat_mode_changed_cb()
 */
int bt_avrcp_set_repeat_mode_changed_cb(bt_avrcp_repeat_mode_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the repeat mode is changed by the remote control device.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_set_repeat_mode_changed_cb()
 */
int bt_avrcp_unset_repeat_mode_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the shuffle mode is changed by the remote control device.
 * @param[in] shuffle The shuffle mode
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_shuffle_mode_changed_cb()
 * @see bt_avrcp_unset_shuffle_mode_changed_cb()
 */
typedef void (*bt_avrcp_shuffle_mode_changed_cb) (bt_avrcp_shuffle_mode_e shuffle, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the shuffle mode is changed by the remote control device.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_shuffle_mode_changed_cb()
 */
int bt_avrcp_set_shuffle_mode_changed_cb(bt_avrcp_shuffle_mode_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the shuffle mode is changed by the remote control device.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_set_shuffle_mode_changed_cb()
 */
int bt_avrcp_unset_shuffle_mode_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the scan mode is changed by the remote control device.
 * @param[in] shuffle The shuffle mode
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_scan_mode_changed_cb()
 * @see bt_avrcp_unset_scan_mode_changed_cb()
 */
typedef void (*bt_avrcp_scan_mode_changed_cb) (bt_avrcp_scan_mode_e scan, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the scan mode is changed by the remote control device.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_scan_mode_changed_cb()
 */
int bt_avrcp_set_scan_mode_changed_cb(bt_avrcp_scan_mode_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the scan mode is changed by the remote control device.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_set_scan_mode_changed_cb()
 */
int bt_avrcp_unset_scan_mode_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief Registers an application that acts as the @a Sink role of HDP(Health Device Profile).
 * @param[in] data_type  The data type of MDEP. This value is defined in ISO/IEEE 11073-20601 spec.
 * For example, pulse oximeter is 0x1004 and blood pressure monitor is 0x1007.
 * @param[out] app_id  The ID of application
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @pre The Bluetooth must be enabled with bt_adapter_enable().
 * @see bt_adapter_enable()
 * @see bt_hdp_deactivate_sink()
 */
int bt_hdp_register_sink_app(unsigned short data_type, char **app_id);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief Unregisters the given application that acts as the @a Sink role of HDP(Health Device Profile).
 * @param[in] app_id  The ID of application
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @see bt_hdp_register_sink_app()
 */
int bt_hdp_unregister_sink_app(const char *app_id);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief Connects the remote device which acts as @a Source role, asynchronously.
 * @param[in] remote_address  The remote address
 * @param[in] app_id  The ID of application
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device is not bonded
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The Sink role of HDP must be activated with bt_hdp_activate_sink().
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @post bt_hdp_connected_cb() will be invoked.
 * @see bt_hdp_disconnect()
 * @see bt_hdp_set_connection_state_changed_cb()
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
int bt_hdp_connect_to_source(const char *remote_address, const char *app_id);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief Disconnects the remote device, asynchronously.
 * @param[in] remote_address  The remote address
 * @param[in] channel  The connected data channel
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The remote device must be connected.
 * @post bt_hdp_disconnected_cb() will be invoked.
 * @see bt_hdp_set_connection_state_changed_cb()
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
int bt_hdp_disconnect(const char *remote_address, unsigned int channel);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief Sends the data to the remote device.
 * @param[in] channel  The connected data channel
 * @param[in] data  The data to send
 * @param[in] size  The size of data to send (byte)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre The remote device must be connected.
 * @see bt_hdp_data_received_cb()
 * @see bt_hdp_set_data_received_cb()
 * @see bt_hdp_unset_data_received_cb()
 */
int bt_hdp_send_data(unsigned int channel, const char *data, unsigned int size);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Called when the connection is established.
 * @param[in] result  The result of connecting to the remote device
 * @param[in] remote_address  The address of connected remote device
 * @param[in] app_id  The ID of application
 * @param[in] type  The type of HDP(Health Device Profile) channel
 * @param[in] channel  The connected data channel
 * @param[in] user_data The user data passed from the callback registration function
 * @see  bt_hdp_disconnected_cb
 * @see bt_hdp_set_connection_state_changed_cb()
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
typedef void (*bt_hdp_connected_cb) (int result, const char *remote_address, const char *app_id,
    bt_hdp_channel_type_e type, unsigned int channel, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Called when the connection is disconnected.
 * @param[in] result  The result of disconnecting from the remote device
 * @param[in] remote_address  The address of disconnected remote device
 * @param[in] channel  The connected data channel
 * @param[in] user_data The user data passed from the callback registration function
 * @see  bt_hdp_connected_cb
 * @see bt_hdp_set_connection_state_changed_cb()
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
typedef void (*bt_hdp_disconnected_cb) (int result, const char *remote_address, unsigned int channel, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state is changed.
 * @param[in] connected_cb The callback function called when a connection is established
 * @param[in] disconnected_cb The callback function called when a connection is disconnected
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
int bt_hdp_set_connection_state_changed_cb(bt_hdp_connected_cb connected_cb, bt_hdp_disconnected_cb disconnected_cb, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state is changed.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_hdp_set_connection_state_changed_cb()
 */
int bt_hdp_unset_connection_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Called when the you receive the data.
 * @param[in] channel  The connected data channel
 * @param[in] data  The received data
 * @param[in] size  The size of received data (byte)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hdp_set_data_received_cb()
 * @see bt_hdp_unset_data_received_cb()
 */
typedef void (*bt_hdp_data_received_cb) (unsigned int channel, const char *data, unsigned int size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Registers a callback function that will be invoked when you receive the data.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_hdp_unset_data_received_cb()
 */
int bt_hdp_set_data_received_cb(bt_hdp_data_received_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Unregisters a callback function that will be invoked when you receive the data.
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_hdp_set_data_received_cb()
 */
int bt_hdp_unset_data_received_cb(void);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when you get the primary services repeatedly.
 * @param[in]  service  The attribute handle of service
 * @param[in]  user_data  The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre  bt_gatt_foreach_primary_services() will invoke this function.
 * @see  bt_gatt_foreach_primary_services()
 */
typedef bool (*bt_gatt_primary_service_cb) (bt_gatt_attribute_h service, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets the primary services of GATT(Generic Attribute Profile).
 * @param[in]  remote_address  The address of the remote device
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval  #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post  @a callback will be called if there are primary services.
 * @see  bt_adapter_enable()
 * @see  bt_gatt_primary_service_cb()
 */
int bt_gatt_foreach_primary_services(const char *remote_address, bt_gatt_primary_service_cb callback, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called after the characteristics are discovered by bt_gatt_discover_characteristics().
 * @remakrs  If bt_gatt_discover_characteristics() failed, then this callback function is called only once with 0 totla and NULL characteristic_handle.
 * @param[in]  result  The result of discovering
 * @param[in]  index  The index of characteristics in a service, starts from 0
 * @param[in]  total  The total number of characteristics in a service
 * @param[in]  characteristic  The attribute handle of characteristic
 * @param[in]  user_data  The user data passed from the request function
 * @return  @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre  bt_gatt_discover_characteristics() will invoke this callback.
 * @see  bt_gatt_discover_characteristics()
 */
typedef bool (*bt_gatt_characteristics_discovered_cb) (int result, int index, int total, bt_gatt_attribute_h characteristic, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Discovers the characteristics in service, asynchronously.
 * @param[in]  service  The attribute handle of service
 * @param[in]  callback  The result callback
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post  @a callback will be called.
 * @see  bt_adapter_enable()
 * @see  bt_gatt_characteristics_discovered_cb()
 */
int bt_gatt_discover_characteristics(bt_gatt_attribute_h service, bt_gatt_characteristics_discovered_cb callback, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets the UUID of service.
 * @remarks  @a uuid must be released with free() by you.
 * @param[in]  service  The attribute handle of service
 * @param[out]  uuid  The UUID of service
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @see  bt_adapter_enable()
 */
int bt_gatt_get_service_uuid(bt_gatt_attribute_h service, char **uuid);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when you get the included services repeatedly.
 * @param[in]  service  The attribute handle of service
 * @param[in]  user_data  The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre  bt_gatt_foreach_included_services() will invoke this function.
 * @see  bt_gatt_foreach_included_services()
 */
typedef bool (*bt_gatt_included_service_cb) (bt_gatt_attribute_h service, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets the included services in service.
 * @param[in]  service  The attribute handle of service
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval  #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post  @a callback will be called if there are included services.
 * @see  bt_adapter_enable()
 * @see  bt_gatt_included_service_cb()
 */
int bt_gatt_foreach_included_services(bt_gatt_attribute_h service, bt_gatt_included_service_cb callback, void *user_data);

/*
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when a characteristic in service is changed.
 * @param[in]  characteristic  The attribute handle of characteristic
 * @param[in]  value  The value of characteristic (byte array)
 * @param[in]  value_length  The length of value
 * @param[in]  user_data  The user data passed from the callback registration function
 * @see bt_gatt_set_characteristic_changed_cb()
 * @see bt_gatt_unset_characteristic_changed_cb()
 */
typedef void (*bt_gatt_characteristic_changed_cb) (bt_gatt_attribute_h characteristic, unsigned char *value, int value_length, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Registers a callback function that will be invoked when a characteristic in service is changed.
 * @param[in]  service  The attribute handle of service
 * @param[in]  callback  The callback function to register
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 * @see  bt_gatt_unset_characteristic_changed_cb()
 */
int bt_gatt_set_characteristic_changed_cb(bt_gatt_attribute_h service, bt_gatt_characteristic_changed_cb callback, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Unregisters a callback function that will be invoked when a characteristic in service is changed.
 * @param[in]  service  The attribute handle of service
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_gatt_set_characteristic_changed_cb()
 */
int bt_gatt_unset_characteristic_changed_cb(bt_gatt_attribute_h service);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets the characteristic declaration.
 * @remarks  @a uuid and @a value must be released with free() by you.
 * @param[in]  characteristic  The attribute handle of characteristic
 * @param[out]  uuid  The UUID of service
 * @param[out]  value  The value of characteristic (byte array)
 * @param[out]  value_length  The length of value
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @see  bt_adapter_enable()
 * @see  bt_gatt_set_characteristic_value()
 */
int bt_gatt_get_characteristic_declaration(bt_gatt_attribute_h characteristic, char **uuid, unsigned char **value, int *value_length);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when a characteristic value is written.
 * @param[in]  characteristic  The attribute handle of characteristic
 * @see bt_gatt_set_characteristic_value_request()
 */
typedef void (*bt_gatt_characteristic_write_cb) (bt_gatt_attribute_h handle);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Sets the value of characteristic.
 * @param[in]  characteristic  The attribute handle of characteristic
 * @param[in]  value  The value of characteristic (byte array)
 * @param[in]  value_length  The length of value
 * @param[in]  request  Request type or command type
 * @param[in]  callback  The result callback
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_OPERATION_FAILED  Operation failed
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @see  bt_adapter_enable()
 * @see  bt_gatt_get_characteristic_declaration()
 */
int bt_gatt_set_characteristic_value_request(bt_gatt_attribute_h characteristic, const unsigned char *value,
				int value_length, unsigned char request, bt_gatt_characteristic_write_cb callback);

/**
* @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
* @brief  Clones the attribute handle.
* @remarks  @a clone must be released with bt_gatt_destroy_attribute_handle().
* @param[out]  clone  The cloned attribute handle
* @param[in]  origin  The origin attribute handle
* @return  0 on success, otherwise negative error value.
* @retval  #BT_ERROR_NONE  Successful
* @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
* @retval  #BT_ERROR_OUT_OF_MEMORY  Out of memory
* @see  bt_gatt_destroy_attribute_handle()
*/
int bt_gatt_clone_attribute_handle(bt_gatt_attribute_h* clone, bt_gatt_attribute_h origin);

/**
* @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
* @brief  Destroys the attribute handle.
* @param[in]  handle  The attribute handle
* @return  0 on success, otherwise negative error value.
* @retval  #BT_ERROR_NONE  Successful
* @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
* @see  bt_gatt_clone_attribute_handle()
*/
int bt_gatt_destroy_attribute_handle(bt_gatt_attribute_h handle);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when a characteristic value is read.
 * @param[in]  value  The value of characteristic (byte array)
 * @param[in]  value_length  The length of value
 * @param[in]  user_data  The user data passed from the foreach function
 * @see bt_gatt_read_characteristic_value()
 */
typedef void (*bt_gatt_characteristic_read_cb) (unsigned char *value,
			int value_length, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Reads the value of characteristic from remote device
 * @param[in]  characteristic  The attribute handle of characteristic
 * @param[in]  callback  The result callback
 * @return	0 on success, otherwise a negative error value.
 * @retval	#BT_ERROR_NONE	Successful
 * @retval	#BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval	#BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval	#BT_ERROR_OPERATION_FAILED	Operation failed
 * @pre  The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @see  bt_adapter_enable()
 * @see  bt_gatt_get_characteristic_declaration()
 */
int bt_gatt_read_characteristic_value(bt_gatt_attribute_h char_handle,
		bt_gatt_characteristic_read_cb callback);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Register bluez agent.
 *
 * @remarks This function must be called before all Bluetooth Agent API.
 * You must free all resources of the Bluetooth agent by calling
 * bt_agent_unregister() if Bluetooth agent is no longer needed.
 *
 * @return  0 on success, otherwise negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_ALREADY_DONE Already register
 *
 * @see  bt_agent_unregister()
 *
 */
int bt_agent_register(bt_agent *agent);

#ifdef TIZEN_3
/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Register bluez agent in syncrhonous mode.
 *
 * @remarks This function is similar to bt_agent_register() but do not register
 * any callback. In that way, user could make some synchronous replies.
 *
 * @return  0 on success, otherwise negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #BT_ERROR_ALREADY_DONE Already register
 *
 * @see  bt_agent_unregister()
 * @see  bt_agent_reply_sync()
 *
 */
int bt_agent_register_sync(void);
#endif
/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief Releases all resources of the Bluetooth Agent.
 *
 * @remarks This function must be called if Bluetooth Agent is no longer needed.
 *
 * @return  0 on success, otherwise negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @see  bt_agent_register()
 */
int bt_agent_unregister(void);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Agent accept confirm info.
 *
 * @param[in]  user_data The user data from agent callback
 * indicate which confirm will be accepted.
 *
 * @see bt_agent_register()
 * @see bt_agent_unregister()
 * @see bt_agent_confirm_reject()
 */
void bt_agent_confirm_accept(bt_req_t *requestion);

#ifdef TIZEN_3
/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Agent synchronous reply.
 *
 * @param[in]  reply The accept or reject user reply which can be
 *
 * @pre to allow sync replies, user should have previously register the agent
 * with bt_agent_register_sync()
 *
 * @see bt_agent_register_sync()
 * @see bt_agent_unregister()
 */
void bt_agent_reply_sync(bt_agent_accept_type_t reply);
#endif
/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Agent reject confirm info.
 *
 * @param[in]  user_data The user data from agent callback
 * indicate which confirm will be rejected.
 *
 * @see bt_agent_register()
 * @see bt_agent_unregister()
 * @see bt_agent_confirm_accept()
 */
void bt_agent_confirm_reject(bt_req_t *requestion);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Agent puts the pin code to authorize.
 *
 * @param[in]  pin_code The pin code used to authorize.
 * @param[in]  user_data The user data from agent callback
 * indicate which pin code authorized will be reply.
 *
 * @see bt_agent_register()
 * @see bt_agent_unregister()
 * @see bt_agent_pincode_cancel()
 */
void bt_agent_pincode_reply(const char *pin_code, bt_req_t *requestion);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AGENT_MODULE
 * @brief  Agent cancel the authorized with pin code.
 *
 * @param[in]  user_data The user data from agent callback
 * indicate which pin code authorized will be reply.
 *
 * @see bt_agent_register()
 * @see bt_agent_unregister()
 * @see bt_agent_pincode_reply()
 */
void bt_agent_pincode_cancel(bt_req_t *requestion);
/**
 * @}
 */
/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when GATT connection state is changed.
 * @param[in]  result  The connection result (1 - Connected 0 - Disconnected)
 * @param[in]  user_data  The user data passed from the foreach function
 * @see bt_device_connect_le()
 * @see bt_device_disconnect_le()
 */
typedef void (*bt_device_gatt_state_changed_cb)(int result, void *user_data);

/**
 * * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * * @brief Connects to remote BLE device, asynchronously.
 *
 * @remarks A Connection can be destroyed by bt_device_disconnect_le()
 *
 * @param[in] callback  The result callback
 * @param[in] address The address of the remote BLE device with
 * which the connection should be created
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY      Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED with bt_adapter_enable()
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @post This function invokes bt_device_gatt_state_changed_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_start_device_discovery()
 */
int bt_device_connect_le(bt_device_gatt_state_changed_cb callback,
			const char *address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Disconnects remote BLE device, asynchronously.
 *
 * @remarks A Connection can be established by bt_device_connect_le()
 *
 * @param[in] callback  The result callback
 * @param[in] address The address of the remote BLE device with
 * which the connection should be created
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY      Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * with bt_adapter_enable()
 * @pre The remote device must be discoverable with
 * bt_adapter_start_device_discovery().
 * @pre The remote device must be connected with bt_device_connect_le().
 * @post This function invokes bt_device_gatt_state_changed_cb().
 *
 * @see bt_adapter_enable()
 * @see bt_adapter_start_device_discovery()
 */
int bt_device_disconnect_le(bt_device_gatt_state_changed_cb callback,
			const char *address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the connectable state changes.
 * @since_tizen 2.3
 *
 * @param[in] result The result of the connectable state changing
 * @param[in] connectable The connectable to be changed
 * @param[in] user_data The user data passed from
 * the callback registration function
 *
 * @pre This function will be invoked when the connectable state of local Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_connectable_changed_cb().
 *
 * @see bt_adapter_set_connectable()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
typedef void (*bt_adapter_connectable_changed_cb)
	(int result, bool connectable, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the connectable state changes.
 * @since_tizen 2.3
 *
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_connectable_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
int bt_adapter_set_connectable_changed_cb(
	bt_adapter_connectable_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Unregisters the callback function.
 * @since_tizen 2.3
 *
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 *
 * @see  bt_initialize()
 * @see  bt_adapter_set_connectable_changed_cb()
 */
int bt_adapter_unset_connectable_changed_cb(void);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Gets the connectable state of local Bluetooth adapter.
 * @since_tizen 2.3
 *
 * @remarks When connectable state is false,
 * no device can connect to this device
 * and visibility mode cannot be changed.
 *
 * @param[out] connectable The connectable state of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_set_connectable()
 */
int bt_adapter_get_connectable(bool *connectable);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Sets the connectable state of local Bluetooth adapter.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks When connectable state is false,
 * no device can connect to this device and visibility mode cannot be changed.
 *
 * @param[in] connectable The connectable state to set
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_connectable_changed_cb() will
 * be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_get_connectable()
 * @see bt_adapter_connectable_changed_cb()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
int bt_adapter_set_connectable(bool connectable);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Get the Hash and Randmoizer value, synchronously.
 * @since_tizen 2.3
 *
 * @param[out] hash The hash value recieved from the controller
 * @param[out] randomizer The hash value recieved from the controller
 * @param[out] hash_len The length of the hash value
 * @param[out] randomizer_len The length of the randomizer value
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 */
int bt_adapter_get_local_oob_data(unsigned char **hash,
					unsigned char **randomizer,
					int *hash_len, int *randomizer_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the Hash and Randmoizer value, synchronously.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 *
 * @param[in] remote_address Remote device address
 * @param[in] hash The hash value recieved from the controller
 * @param[in] randomizer The hash value recieved from the controller
 * @param[in] hash_len The length of the hash value. Allowed value is 16
 * @param[in] randomizer_len The length of the randomizer value. Allowed value is 16
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 */
int bt_adapter_set_remote_oob_data(const char *remote_address,
				unsigned char *hash,
				unsigned char *randomizer,
				int hash_len, int randomizer_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Deletes the Hash and Randomizer value, synchronously.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 *
 * @param[in] remote_address Remote device address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 */
int bt_adapter_remove_remote_oob_data(const char *remote_address);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __TIZEN_NETWORK_BLUETOOTH_H__
