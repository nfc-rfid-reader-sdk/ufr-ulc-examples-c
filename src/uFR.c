/*
 * uFR.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <uFCoder.h>
#include "ini.h"
#include "uFR.h"

//------------------------------------------------------------------------------
bool CheckDependencies(void) {
#if defined(EXIT_ON_WRONG_FW_DEPENDENCY) || defined(EXIT_ON_WRONG_LIB_DEPENDENCY)
	uint8_t version_major, version_minor, build;
	bool wrong_version = false;
#endif
	UFR_STATUS status;

#ifdef EXIT_ON_WRONG_LIB_DEPENDENCY
	uint32_t dwDllVersion = 0;

	dwDllVersion = GetDllVersion();

	// "explode" the uFCoder library version:
	version_major = (uint8_t)dwDllVersion;
	version_minor = (uint8_t)(dwDllVersion >> 8);

	// Get the uFCoder library build number.
	build = (uint8_t)(dwDllVersion >> 16);

	if (version_major < MIN_DEPEND_LIB_VER_MAJOR) {
		wrong_version = true;
	} else if (version_major == MIN_DEPEND_LIB_VER_MAJOR && version_minor < MIN_DEPEND_LIB_VER_MINOR) {
		wrong_version = true;
	} else if (version_major == MIN_DEPEND_LIB_VER_MAJOR && version_minor == MIN_DEPEND_LIB_VER_MINOR && build < MIN_DEPEND_LIB_VER_BUILD) {
		wrong_version = true;
	}

	if (wrong_version) {
		printf("Wrong uFCoder library version (%d.%d.%d).\n"
			   "Please update uFCoder library to at last %d.%d.%d version.\n",
			   version_major, version_minor, build,
			   MIN_DEPEND_LIB_VER_MAJOR, MIN_DEPEND_LIB_VER_MINOR, MIN_DEPEND_LIB_VER_BUILD);
		return false;
	}
#endif
#ifdef EXIT_ON_WRONG_FW_DEPENDENCY
	wrong_version = false;
	status = GetReaderFirmwareVersion(&version_major, &version_minor);
	if (status != UFR_OK) {
		printf("Error while checking firmware version, status is: 0x%08X\n", status);
	}
	status = GetBuildNumber(&build);

	if (status != UFR_OK) {
		printf("Error while firmware version, status is: 0x%08X\n", status);
	}
	if (version_major < MIN_DEPEND_FW_VER_MAJOR) {
		wrong_version = true;
	} else if (version_major == MIN_DEPEND_FW_VER_MAJOR && version_minor < MIN_DEPEND_FW_VER_MINOR) {
		wrong_version = true;
	} else if (version_major == MIN_DEPEND_FW_VER_MAJOR && version_minor == MIN_DEPEND_FW_VER_MINOR && build < MIN_DEPEND_FW_VER_BUILD) {
		wrong_version = true;
	}

	if (wrong_version) {
		printf("Wrong uFR NFC reader firmware version (%d.%d.%d).\n"
			   "Please update uFR firmware to at last %d.%d.%d version.\n",
			   version_major, version_minor, build,
			   MIN_DEPEND_FW_VER_MAJOR, MIN_DEPEND_FW_VER_MINOR, MIN_DEPEND_FW_VER_BUILD);
		return false;
	}
#endif
	return true;
}
//------------------------------------------------------------------------------
sz_ptr GetDlTypeName(uint8_t dl_type_code) {

	switch (dl_type_code) {
		case DL_MIFARE_ULTRALIGHT:
			return "DL_MIFARE_ULTRALIGHT";
		case DL_MIFARE_ULTRALIGHT_EV1_11:
			return "DL_MIFARE_ULTRALIGHT_EV1_11";
		case DL_MIFARE_ULTRALIGHT_EV1_21:
			return "DL_MIFARE_ULTRALIGHT_EV1_21";
		case DL_MIFARE_ULTRALIGHT_C:
			return "DL_MIFARE_ULTRALIGHT_C";
		case DL_NTAG_203:
			return "DL_NTAG_203";
		case DL_NTAG_210:
			return "DL_NTAG_210";
		case DL_NTAG_212:
			return "DL_NTAG_212";
		case DL_NTAG_213:
			return "DL_NTAG_213";
		case DL_NTAG_215:
			return "DL_NTAG_215";
		case DL_NTAG_216:
			return "DL_NTAG_216";
		case DL_MIKRON_MIK640D:
			return "DL_MIKRON_MIK640D";
		case DL_MIFARE_MINI:
			return "DL_MIFARE_MINI";
		case DL_MIFARE_CLASSIC_1K:
			return "DL_MIFARE_CLASSIC_1K";
		case DL_MIFARE_CLASSIC_4K:
			return "DL_MIFARE_CLASSIC_4K";
		case DL_MIFARE_PLUS_S_2K_SL0:
			return "DL_MIFARE_PLUS_S_2K_SL0";
		case DL_MIFARE_PLUS_S_4K_SL0:
			return "DL_MIFARE_PLUS_S_4K_SL0";
		case DL_MIFARE_PLUS_X_2K_SL0:
			return "DL_MIFARE_PLUS_X_2K_SL0";
		case DL_MIFARE_PLUS_X_4K_SL0:
			return "DL_MIFARE_PLUS_X_4K_SL0";
		case DL_MIFARE_DESFIRE:
			return "DL_MIFARE_DESFIRE";
		case DL_MIFARE_DESFIRE_EV1_2K:
			return "DL_MIFARE_DESFIRE_EV1_2K";
		case DL_MIFARE_DESFIRE_EV1_4K:
			return "DL_MIFARE_DESFIRE_EV1_4K";
		case DL_MIFARE_DESFIRE_EV1_8K:
			return "DL_MIFARE_DESFIRE_EV1_8K";
		case DL_MIFARE_DESFIRE_EV2_2K:
			return "DL_MIFARE_DESFIRE_EV2_2K";
		case DL_MIFARE_DESFIRE_EV2_4K:
			return "DL_MIFARE_DESFIRE_EV2_4K";
		case DL_MIFARE_DESFIRE_EV2_8K:
			return "DL_MIFARE_DESFIRE_EV2_8K";
		case DL_MIFARE_PLUS_S_2K_SL1:
			return "DL_MIFARE_PLUS_S_2K_SL1";
		case DL_MIFARE_PLUS_X_2K_SL1:
			return "DL_MIFARE_PLUS_X_2K_SL1";
		case DL_MIFARE_PLUS_EV1_2K_SL1:
			return "DL_MIFARE_PLUS_EV1_2K_SL1";
		case DL_MIFARE_PLUS_X_2K_SL2:
			return "DL_MIFARE_PLUS_X_2K_SL2";
		case DL_MIFARE_PLUS_S_2K_SL3:
			return "DL_MIFARE_PLUS_S_2K_SL3";
		case DL_MIFARE_PLUS_X_2K_SL3:
			return "DL_MIFARE_PLUS_X_2K_SL3";
		case DL_MIFARE_PLUS_EV1_2K_SL3:
			return "DL_MIFARE_PLUS_EV1_2K_SL3";
		case DL_MIFARE_PLUS_S_4K_SL1:
			return "DL_MIFARE_PLUS_S_4K_SL1";
		case DL_MIFARE_PLUS_X_4K_SL1:
			return "DL_MIFARE_PLUS_X_4K_SL1";
		case DL_MIFARE_PLUS_EV1_4K_SL1:
			return "DL_MIFARE_PLUS_EV1_4K_SL1";
		case DL_MIFARE_PLUS_X_4K_SL2:
			return "DL_MIFARE_PLUS_X_4K_SL2";
		case DL_MIFARE_PLUS_S_4K_SL3:
			return "DL_MIFARE_PLUS_S_4K_SL3";
		case DL_MIFARE_PLUS_X_4K_SL3:
			return "DL_MIFARE_PLUS_X_4K_SL3";
		case DL_MIFARE_PLUS_EV1_4K_SL3:
			return "DL_MIFARE_PLUS_EV1_4K_SL3";
		case DL_GENERIC_ISO14443_4:
			return "DL_GENERIC_ISO_14443_4";
		case DL_GENERIC_ISO14443_4_TYPE_B:
			return "DL_GENERIC_ISO14443_4_TYPE_B";
		case DL_GENERIC_ISO14443_3_TYPE_B:
			return "DL_GENERIC_ISO14443_3_TYPE_B";
		case DL_IMEI_UID:
			return "DL_IMEI_UID";
		}
		return "UNKNOWN CARD";
}
//------------------------------------------------------------------------------
