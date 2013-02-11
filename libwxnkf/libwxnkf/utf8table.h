/*
 * utf8table.h
 *
 *  Created on: 2012/07/10
 * Contributor: Hiroyuki Nagata
 */

#ifndef UTF8TABLE_H_
#define UTF8TABLE_H_

#define sizeof_euc_to_utf8_1byte 94
#define sizeof_euc_to_utf8_2bytes 94
#define sizeof_utf8_to_euc_C2 64
#define sizeof_utf8_to_euc_E5B8 64
#define sizeof_utf8_to_euc_2bytes 112
#define sizeof_utf8_to_euc_3bytes 16

/**
 * UNICODE_NORMALIZATION
 */
#define NORMALIZATION_TABLE_LENGTH 942
#define NORMALIZATION_TABLE_NFC_LENGTH 3
#define NORMALIZATION_TABLE_NFD_LENGTH 9

struct normalization_pair {
	const unsigned char nfc[NORMALIZATION_TABLE_NFC_LENGTH];
	const unsigned char nfd[NORMALIZATION_TABLE_NFD_LENGTH];
};

#define CP932_TABLE_BEGIN 0xFA
#define CP932_TABLE_END   0xFC
#define CP932INV_TABLE_BEGIN 0xED
#define CP932INV_TABLE_END   0xEE


class UTF8Table {

public:

	// declare several utf8table array
	static const unsigned short euc_to_utf8_A1[];
	static const unsigned short euc_to_utf8_A1_ms[];
	static const unsigned short euc_to_utf8_A2[];
	static const unsigned short euc_to_utf8_A2_ms[];
	static const unsigned short euc_to_utf8_A3[];
	static const unsigned short euc_to_utf8_A4[];
	static const unsigned short euc_to_utf8_A5[];
	static const unsigned short euc_to_utf8_A6[];
	static const unsigned short euc_to_utf8_A7[];
	static const unsigned short euc_to_utf8_A8[];
	static const unsigned short euc_to_utf8_A9[];
	static const unsigned short euc_to_utf8_AA[];
	static const unsigned short euc_to_utf8_AB[];
	static const unsigned short euc_to_utf8_AC[];
	static const unsigned short euc_to_utf8_AC_mac[];
	static const unsigned short euc_to_utf8_AD[];
	static const unsigned short euc_to_utf8_AD_mac[];
	static const unsigned short euc_to_utf8_AE[];
	static const unsigned short euc_to_utf8_AF[];
	static const unsigned short euc_to_utf8_B0[];
	static const unsigned short euc_to_utf8_B1[];
	static const unsigned short euc_to_utf8_B2[];
	static const unsigned short euc_to_utf8_B3[];
	static const unsigned short euc_to_utf8_B4[];
	static const unsigned short euc_to_utf8_B5[];
	static const unsigned short euc_to_utf8_B6[];
	static const unsigned short euc_to_utf8_B7[];
	static const unsigned short euc_to_utf8_B8[];
	static const unsigned short euc_to_utf8_B9[];
	static const unsigned short euc_to_utf8_BA[];
	static const unsigned short euc_to_utf8_BB[];
	static const unsigned short euc_to_utf8_BC[];
	static const unsigned short euc_to_utf8_BD[];
	static const unsigned short euc_to_utf8_BE[];
	static const unsigned short euc_to_utf8_BF[];
	static const unsigned short euc_to_utf8_C0[];
	static const unsigned short euc_to_utf8_C1[];
	static const unsigned short euc_to_utf8_C2[];
	static const unsigned short euc_to_utf8_C3[];
	static const unsigned short euc_to_utf8_C4[];
	static const unsigned short euc_to_utf8_C5[];
	static const unsigned short euc_to_utf8_C6[];
	static const unsigned short euc_to_utf8_C7[];
	static const unsigned short euc_to_utf8_C8[];
	static const unsigned short euc_to_utf8_C9[];
	static const unsigned short euc_to_utf8_CA[];
	static const unsigned short euc_to_utf8_CB[];
	static const unsigned short euc_to_utf8_CC[];
	static const unsigned short euc_to_utf8_CD[];
	static const unsigned short euc_to_utf8_CE[];
	static const unsigned short euc_to_utf8_CF[];
	static const unsigned short euc_to_utf8_D0[];
	static const unsigned short euc_to_utf8_D1[];
	static const unsigned short euc_to_utf8_D2[];
	static const unsigned short euc_to_utf8_D3[];
	static const unsigned short euc_to_utf8_D4[];
	static const unsigned short euc_to_utf8_D5[];
	static const unsigned short euc_to_utf8_D6[];
	static const unsigned short euc_to_utf8_D7[];
	static const unsigned short euc_to_utf8_D8[];
	static const unsigned short euc_to_utf8_D9[];
	static const unsigned short euc_to_utf8_DA[];
	static const unsigned short euc_to_utf8_DB[];
	static const unsigned short euc_to_utf8_DC[];
	static const unsigned short euc_to_utf8_DD[];
	static const unsigned short euc_to_utf8_DE[];
	static const unsigned short euc_to_utf8_DF[];
	static const unsigned short euc_to_utf8_E0[];
	static const unsigned short euc_to_utf8_E1[];
	static const unsigned short euc_to_utf8_E2[];
	static const unsigned short euc_to_utf8_E3[];
	static const unsigned short euc_to_utf8_E4[];
	static const unsigned short euc_to_utf8_E5[];
	static const unsigned short euc_to_utf8_E6[];
	static const unsigned short euc_to_utf8_E7[];
	static const unsigned short euc_to_utf8_E8[];
	static const unsigned short euc_to_utf8_E9[];
	static const unsigned short euc_to_utf8_EA[];
	static const unsigned short euc_to_utf8_EB[];
	static const unsigned short euc_to_utf8_EC[];
	static const unsigned short euc_to_utf8_ED[];
	static const unsigned short euc_to_utf8_EE[];
	static const unsigned short euc_to_utf8_EF[];
	static const unsigned short euc_to_utf8_F0[];
	static const unsigned short euc_to_utf8_F1[];
	static const unsigned short euc_to_utf8_F2[];
	static const unsigned short euc_to_utf8_F3[];
	static const unsigned short euc_to_utf8_F4[];
	static const unsigned short euc_to_utf8_F5[];
	static const unsigned short euc_to_utf8_F9[];
	static const unsigned short euc_to_utf8_FA[];
	static const unsigned short euc_to_utf8_FB[];
	static const unsigned short euc_to_utf8_FC[];
	static const unsigned short euc_to_utf8_FC_ms[];
	static const unsigned short euc_to_utf8_8FA2[];
	static const unsigned short euc_to_utf8_8FA6[];
	static const unsigned short euc_to_utf8_8FA7[];
	static const unsigned short euc_to_utf8_8FA9[];
	static const unsigned short euc_to_utf8_8FAA[];
	static const unsigned short euc_to_utf8_8FAB[];
	static const unsigned short euc_to_utf8_8FB0[];
	static const unsigned short euc_to_utf8_8FB1[];
	static const unsigned short euc_to_utf8_8FB2[];
	static const unsigned short euc_to_utf8_8FB3[];
	static const unsigned short euc_to_utf8_8FB4[];
	static const unsigned short euc_to_utf8_8FB5[];
	static const unsigned short euc_to_utf8_8FB6[];
	static const unsigned short euc_to_utf8_8FB7[];
	static const unsigned short euc_to_utf8_8FB8[];
	static const unsigned short euc_to_utf8_8FB9[];
	static const unsigned short euc_to_utf8_8FBA[];
	static const unsigned short euc_to_utf8_8FBB[];
	static const unsigned short euc_to_utf8_8FBC[];
	static const unsigned short euc_to_utf8_8FBD[];
	static const unsigned short euc_to_utf8_8FBE[];
	static const unsigned short euc_to_utf8_8FBF[];
	static const unsigned short euc_to_utf8_8FC0[];
	static const unsigned short euc_to_utf8_8FC1[];
	static const unsigned short euc_to_utf8_8FC2[];
	static const unsigned short euc_to_utf8_8FC3[];
	static const unsigned short euc_to_utf8_8FC4[];
	static const unsigned short euc_to_utf8_8FC5[];
	static const unsigned short euc_to_utf8_8FC6[];
	static const unsigned short euc_to_utf8_8FC7[];
	static const unsigned short euc_to_utf8_8FC8[];
	static const unsigned short euc_to_utf8_8FC9[];
	static const unsigned short euc_to_utf8_8FCA[];
	static const unsigned short euc_to_utf8_8FCB[];
	static const unsigned short euc_to_utf8_8FCC[];
	static const unsigned short euc_to_utf8_8FCD[];
	static const unsigned short euc_to_utf8_8FCE[];
	static const unsigned short euc_to_utf8_8FCF[];
	static const unsigned short euc_to_utf8_8FD0[];
	static const unsigned short euc_to_utf8_8FD1[];
	static const unsigned short euc_to_utf8_8FD2[];
	static const unsigned short euc_to_utf8_8FD3[];
	static const unsigned short euc_to_utf8_8FD4[];
	static const unsigned short euc_to_utf8_8FD5[];
	static const unsigned short euc_to_utf8_8FD6[];
	static const unsigned short euc_to_utf8_8FD7[];
	static const unsigned short euc_to_utf8_8FD8[];
	static const unsigned short euc_to_utf8_8FD9[];
	static const unsigned short euc_to_utf8_8FDA[];
	static const unsigned short euc_to_utf8_8FDB[];
	static const unsigned short euc_to_utf8_8FDC[];
	static const unsigned short euc_to_utf8_8FDD[];
	static const unsigned short euc_to_utf8_8FDE[];
	static const unsigned short euc_to_utf8_8FDF[];
	static const unsigned short euc_to_utf8_8FE0[];
	static const unsigned short euc_to_utf8_8FE1[];
	static const unsigned short euc_to_utf8_8FE2[];
	static const unsigned short euc_to_utf8_8FE3[];
	static const unsigned short euc_to_utf8_8FE4[];
	static const unsigned short euc_to_utf8_8FE5[];
	static const unsigned short euc_to_utf8_8FE6[];
	static const unsigned short euc_to_utf8_8FE7[];
	static const unsigned short euc_to_utf8_8FE8[];
	static const unsigned short euc_to_utf8_8FE9[];
	static const unsigned short euc_to_utf8_8FEA[];
	static const unsigned short euc_to_utf8_8FEB[];
	static const unsigned short euc_to_utf8_8FEC[];
	static const unsigned short euc_to_utf8_8FED[];
	static const unsigned short euc_to_utf8_8FF3[];
	static const unsigned short euc_to_utf8_8FF4[];
	static const unsigned short utf8_to_euc_C2[];
	static const unsigned short utf8_to_euc_C2_ms[];
	static const unsigned short utf8_to_euc_C2_mac[];
	static const unsigned short utf8_to_euc_C2_932[];
	static const unsigned short utf8_to_euc_C3[];
	static const unsigned short utf8_to_euc_C3_932[];
	static const unsigned short utf8_to_euc_C4[];
	static const unsigned short utf8_to_euc_C5[];
	static const unsigned short utf8_to_euc_C7[];
	static const unsigned short utf8_to_euc_CB[];
	static const unsigned short utf8_to_euc_CE[];
	static const unsigned short utf8_to_euc_CF[];
	static const unsigned short utf8_to_euc_D0[];
	static const unsigned short utf8_to_euc_D1[];
	static const unsigned short utf8_to_euc_E280[];
	static const unsigned short utf8_to_euc_E280_ms[];
	static const unsigned short utf8_to_euc_E280_932[];
	static const unsigned short utf8_to_euc_E284[];
	static const unsigned short utf8_to_euc_E284_mac[];
	static const unsigned short utf8_to_euc_E285[];
	static const unsigned short utf8_to_euc_E285_mac[];
	static const unsigned short utf8_to_euc_E286[];
	static const unsigned short utf8_to_euc_E287[];
	static const unsigned short utf8_to_euc_E288[];
	static const unsigned short utf8_to_euc_E288_932[];
	static const unsigned short utf8_to_euc_E288_mac[];
	static const unsigned short utf8_to_euc_E289[];
	static const unsigned short utf8_to_euc_E28A[];
	static const unsigned short utf8_to_euc_E28A_mac[];
	static const unsigned short utf8_to_euc_E28C[];
	static const unsigned short utf8_to_euc_E291[];
	static const unsigned short utf8_to_euc_E291_mac[];
	static const unsigned short utf8_to_euc_E294[];
	static const unsigned short utf8_to_euc_E295[];
	static const unsigned short utf8_to_euc_E296[];
	static const unsigned short utf8_to_euc_E297[];
	static const unsigned short utf8_to_euc_E298[];
	static const unsigned short utf8_to_euc_E299[];
	static const unsigned short utf8_to_euc_E380[];
	static const unsigned short utf8_to_euc_E380_932[];
	static const unsigned short utf8_to_euc_E381[];
	static const unsigned short utf8_to_euc_E382[];
	static const unsigned short utf8_to_euc_E382_932[];
	static const unsigned short utf8_to_euc_E383[];
	static const unsigned short utf8_to_euc_E388[];
	static const unsigned short utf8_to_euc_E388_mac[];
	static const unsigned short utf8_to_euc_E38A[];
	static const unsigned short utf8_to_euc_E38A_mac[];
	static const unsigned short utf8_to_euc_E38C[];
	static const unsigned short utf8_to_euc_E38C_mac[];
	static const unsigned short utf8_to_euc_E38D[];
	static const unsigned short utf8_to_euc_E38D_mac[];
	static const unsigned short utf8_to_euc_E38E[];
	static const unsigned short utf8_to_euc_E38E_mac[];
	static const unsigned short utf8_to_euc_E38F[];
	static const unsigned short utf8_to_euc_E38F_mac[];
	static const unsigned short utf8_to_euc_E4B8[];
	static const unsigned short utf8_to_euc_E4B9[];
	static const unsigned short utf8_to_euc_E4BA[];
	static const unsigned short utf8_to_euc_E4BB[];
	static const unsigned short utf8_to_euc_E4BC[];
	static const unsigned short utf8_to_euc_E4BD[];
	static const unsigned short utf8_to_euc_E4BE[];
	static const unsigned short utf8_to_euc_E4BF[];
	static const unsigned short utf8_to_euc_E580[];
	static const unsigned short utf8_to_euc_E581[];
	static const unsigned short utf8_to_euc_E582[];
	static const unsigned short utf8_to_euc_E583[];
	static const unsigned short utf8_to_euc_E584[];
	static const unsigned short utf8_to_euc_E585[];
	static const unsigned short utf8_to_euc_E586[];
	static const unsigned short utf8_to_euc_E587[];
	static const unsigned short utf8_to_euc_E588[];
	static const unsigned short utf8_to_euc_E589[];
	static const unsigned short utf8_to_euc_E58A[];
	static const unsigned short utf8_to_euc_E58B[];
	static const unsigned short utf8_to_euc_E58C[];
	static const unsigned short utf8_to_euc_E58D[];
	static const unsigned short utf8_to_euc_E58E[];
	static const unsigned short utf8_to_euc_E58F[];
	static const unsigned short utf8_to_euc_E590[];
	static const unsigned short utf8_to_euc_E591[];
	static const unsigned short utf8_to_euc_E592[];
	static const unsigned short utf8_to_euc_E593[];
	static const unsigned short utf8_to_euc_E594[];
	static const unsigned short utf8_to_euc_E595[];
	static const unsigned short utf8_to_euc_E596[];
	static const unsigned short utf8_to_euc_E597[];
	static const unsigned short utf8_to_euc_E598[];
	static const unsigned short utf8_to_euc_E599[];
	static const unsigned short utf8_to_euc_E59A[];
	static const unsigned short utf8_to_euc_E59B[];
	static const unsigned short utf8_to_euc_E59C[];
	static const unsigned short utf8_to_euc_E59D[];
	static const unsigned short utf8_to_euc_E59E[];
	static const unsigned short utf8_to_euc_E59F[];
	static const unsigned short utf8_to_euc_E5A0[];
	static const unsigned short utf8_to_euc_E5A1[];
	static const unsigned short utf8_to_euc_E5A2[];
	static const unsigned short utf8_to_euc_E5A3[];
	static const unsigned short utf8_to_euc_E5A4[];
	static const unsigned short utf8_to_euc_E5A5[];
	static const unsigned short utf8_to_euc_E5A6[];
	static const unsigned short utf8_to_euc_E5A7[];
	static const unsigned short utf8_to_euc_E5A8[];
	static const unsigned short utf8_to_euc_E5A9[];
	static const unsigned short utf8_to_euc_E5AA[];
	static const unsigned short utf8_to_euc_E5AB[];
	static const unsigned short utf8_to_euc_E5AC[];
	static const unsigned short utf8_to_euc_E5AD[];
	static const unsigned short utf8_to_euc_E5AE[];
	static const unsigned short utf8_to_euc_E5AF[];
	static const unsigned short utf8_to_euc_E5B0[];
	static const unsigned short utf8_to_euc_E5B1[];
	static const unsigned short utf8_to_euc_E5B2[];
	static const unsigned short utf8_to_euc_E5B3[];
	static const unsigned short utf8_to_euc_E5B4[];
	static const unsigned short utf8_to_euc_E5B5[];
	static const unsigned short utf8_to_euc_E5B6[];
	static const unsigned short utf8_to_euc_E5B7[];
	static const unsigned short utf8_to_euc_E5B8[];
	static const unsigned short utf8_to_euc_E5B9[];
	static const unsigned short utf8_to_euc_E5BA[];
	static const unsigned short utf8_to_euc_E5BB[];
	static const unsigned short utf8_to_euc_E5BC[];
	static const unsigned short utf8_to_euc_E5BD[];
	static const unsigned short utf8_to_euc_E5BE[];
	static const unsigned short utf8_to_euc_E5BF[];
	static const unsigned short utf8_to_euc_E680[];
	static const unsigned short utf8_to_euc_E681[];
	static const unsigned short utf8_to_euc_E682[];
	static const unsigned short utf8_to_euc_E683[];
	static const unsigned short utf8_to_euc_E684[];
	static const unsigned short utf8_to_euc_E685[];
	static const unsigned short utf8_to_euc_E686[];
	static const unsigned short utf8_to_euc_E687[];
	static const unsigned short utf8_to_euc_E688[];
	static const unsigned short utf8_to_euc_E689[];
	static const unsigned short utf8_to_euc_E68A[];
	static const unsigned short utf8_to_euc_E68B[];
	static const unsigned short utf8_to_euc_E68C[];
	static const unsigned short utf8_to_euc_E68D[];
	static const unsigned short utf8_to_euc_E68E[];
	static const unsigned short utf8_to_euc_E68F[];
	static const unsigned short utf8_to_euc_E690[];
	static const unsigned short utf8_to_euc_E691[];
	static const unsigned short utf8_to_euc_E692[];
	static const unsigned short utf8_to_euc_E693[];
	static const unsigned short utf8_to_euc_E694[];
	static const unsigned short utf8_to_euc_E695[];
	static const unsigned short utf8_to_euc_E696[];
	static const unsigned short utf8_to_euc_E697[];
	static const unsigned short utf8_to_euc_E698[];
	static const unsigned short utf8_to_euc_E699[];
	static const unsigned short utf8_to_euc_E69A[];
	static const unsigned short utf8_to_euc_E69B[];
	static const unsigned short utf8_to_euc_E69C[];
	static const unsigned short utf8_to_euc_E69D[];
	static const unsigned short utf8_to_euc_E69E[];
	static const unsigned short utf8_to_euc_E69F[];
	static const unsigned short utf8_to_euc_E6A0[];
	static const unsigned short utf8_to_euc_E6A1[];
	static const unsigned short utf8_to_euc_E6A2[];
	static const unsigned short utf8_to_euc_E6A3[];
	static const unsigned short utf8_to_euc_E6A4[];
	static const unsigned short utf8_to_euc_E6A5[];
	static const unsigned short utf8_to_euc_E6A6[];
	static const unsigned short utf8_to_euc_E6A7[];
	static const unsigned short utf8_to_euc_E6A8[];
	static const unsigned short utf8_to_euc_E6A9[];
	static const unsigned short utf8_to_euc_E6AA[];
	static const unsigned short utf8_to_euc_E6AB[];
	static const unsigned short utf8_to_euc_E6AC[];
	static const unsigned short utf8_to_euc_E6AD[];
	static const unsigned short utf8_to_euc_E6AE[];
	static const unsigned short utf8_to_euc_E6AF[];
	static const unsigned short utf8_to_euc_E6B0[];
	static const unsigned short utf8_to_euc_E6B1[];
	static const unsigned short utf8_to_euc_E6B2[];
	static const unsigned short utf8_to_euc_E6B3[];
	static const unsigned short utf8_to_euc_E6B4[];
	static const unsigned short utf8_to_euc_E6B5[];
	static const unsigned short utf8_to_euc_E6B6[];
	static const unsigned short utf8_to_euc_E6B7[];
	static const unsigned short utf8_to_euc_E6B8[];
	static const unsigned short utf8_to_euc_E6B9[];
	static const unsigned short utf8_to_euc_E6BA[];
	static const unsigned short utf8_to_euc_E6BB[];
	static const unsigned short utf8_to_euc_E6BC[];
	static const unsigned short utf8_to_euc_E6BD[];
	static const unsigned short utf8_to_euc_E6BE[];
	static const unsigned short utf8_to_euc_E6BF[];
	static const unsigned short utf8_to_euc_E780[];
	static const unsigned short utf8_to_euc_E781[];
	static const unsigned short utf8_to_euc_E782[];
	static const unsigned short utf8_to_euc_E783[];
	static const unsigned short utf8_to_euc_E784[];
	static const unsigned short utf8_to_euc_E785[];
	static const unsigned short utf8_to_euc_E786[];
	static const unsigned short utf8_to_euc_E787[];
	static const unsigned short utf8_to_euc_E788[];
	static const unsigned short utf8_to_euc_E789[];
	static const unsigned short utf8_to_euc_E78A[];
	static const unsigned short utf8_to_euc_E78B[];
	static const unsigned short utf8_to_euc_E78C[];
	static const unsigned short utf8_to_euc_E78D[];
	static const unsigned short utf8_to_euc_E78E[];
	static const unsigned short utf8_to_euc_E78F[];
	static const unsigned short utf8_to_euc_E790[];
	static const unsigned short utf8_to_euc_E791[];
	static const unsigned short utf8_to_euc_E792[];
	static const unsigned short utf8_to_euc_E793[];
	static const unsigned short utf8_to_euc_E794[];
	static const unsigned short utf8_to_euc_E795[];
	static const unsigned short utf8_to_euc_E796[];
	static const unsigned short utf8_to_euc_E797[];
	static const unsigned short utf8_to_euc_E798[];
	static const unsigned short utf8_to_euc_E799[];
	static const unsigned short utf8_to_euc_E79A[];
	static const unsigned short utf8_to_euc_E79B[];
	static const unsigned short utf8_to_euc_E79C[];
	static const unsigned short utf8_to_euc_E79D[];
	static const unsigned short utf8_to_euc_E79E[];
	static const unsigned short utf8_to_euc_E79F[];
	static const unsigned short utf8_to_euc_E7A0[];
	static const unsigned short utf8_to_euc_E7A1[];
	static const unsigned short utf8_to_euc_E7A2[];
	static const unsigned short utf8_to_euc_E7A3[];
	static const unsigned short utf8_to_euc_E7A4[];
	static const unsigned short utf8_to_euc_E7A5[];
	static const unsigned short utf8_to_euc_E7A6[];
	static const unsigned short utf8_to_euc_E7A7[];
	static const unsigned short utf8_to_euc_E7A8[];
	static const unsigned short utf8_to_euc_E7A9[];
	static const unsigned short utf8_to_euc_E7AA[];
	static const unsigned short utf8_to_euc_E7AB[];
	static const unsigned short utf8_to_euc_E7AC[];
	static const unsigned short utf8_to_euc_E7AD[];
	static const unsigned short utf8_to_euc_E7AE[];
	static const unsigned short utf8_to_euc_E7AF[];
	static const unsigned short utf8_to_euc_E7B0[];
	static const unsigned short utf8_to_euc_E7B1[];
	static const unsigned short utf8_to_euc_E7B2[];
	static const unsigned short utf8_to_euc_E7B3[];
	static const unsigned short utf8_to_euc_E7B4[];
	static const unsigned short utf8_to_euc_E7B5[];
	static const unsigned short utf8_to_euc_E7B6[];
	static const unsigned short utf8_to_euc_E7B7[];
	static const unsigned short utf8_to_euc_E7B8[];
	static const unsigned short utf8_to_euc_E7B9[];
	static const unsigned short utf8_to_euc_E7BA[];
	static const unsigned short utf8_to_euc_E7BC[];
	static const unsigned short utf8_to_euc_E7BD[];
	static const unsigned short utf8_to_euc_E7BE[];
	static const unsigned short utf8_to_euc_E7BF[];
	static const unsigned short utf8_to_euc_E880[];
	static const unsigned short utf8_to_euc_E881[];
	static const unsigned short utf8_to_euc_E882[];
	static const unsigned short utf8_to_euc_E883[];
	static const unsigned short utf8_to_euc_E884[];
	static const unsigned short utf8_to_euc_E885[];
	static const unsigned short utf8_to_euc_E886[];
	static const unsigned short utf8_to_euc_E887[];
	static const unsigned short utf8_to_euc_E888[];
	static const unsigned short utf8_to_euc_E889[];
	static const unsigned short utf8_to_euc_E88A[];
	static const unsigned short utf8_to_euc_E88B[];
	static const unsigned short utf8_to_euc_E88C[];
	static const unsigned short utf8_to_euc_E88D[];
	static const unsigned short utf8_to_euc_E88E[];
	static const unsigned short utf8_to_euc_E88F[];
	static const unsigned short utf8_to_euc_E890[];
	static const unsigned short utf8_to_euc_E891[];
	static const unsigned short utf8_to_euc_E892[];
	static const unsigned short utf8_to_euc_E893[];
	static const unsigned short utf8_to_euc_E894[];
	static const unsigned short utf8_to_euc_E895[];
	static const unsigned short utf8_to_euc_E896[];
	static const unsigned short utf8_to_euc_E897[];
	static const unsigned short utf8_to_euc_E898[];
	static const unsigned short utf8_to_euc_E899[];
	static const unsigned short utf8_to_euc_E89A[];
	static const unsigned short utf8_to_euc_E89B[];
	static const unsigned short utf8_to_euc_E89C[];
	static const unsigned short utf8_to_euc_E89D[];
	static const unsigned short utf8_to_euc_E89E[];
	static const unsigned short utf8_to_euc_E89F[];
	static const unsigned short utf8_to_euc_E8A0[];
	static const unsigned short utf8_to_euc_E8A1[];
	static const unsigned short utf8_to_euc_E8A2[];
	static const unsigned short utf8_to_euc_E8A3[];
	static const unsigned short utf8_to_euc_E8A4[];
	static const unsigned short utf8_to_euc_E8A5[];
	static const unsigned short utf8_to_euc_E8A6[];
	static const unsigned short utf8_to_euc_E8A7[];
	static const unsigned short utf8_to_euc_E8A8[];
	static const unsigned short utf8_to_euc_E8A9[];
	static const unsigned short utf8_to_euc_E8AA[];
	static const unsigned short utf8_to_euc_E8AB[];
	static const unsigned short utf8_to_euc_E8AC[];
	static const unsigned short utf8_to_euc_E8AD[];
	static const unsigned short utf8_to_euc_E8AE[];
	static const unsigned short utf8_to_euc_E8B0[];
	static const unsigned short utf8_to_euc_E8B1[];
	static const unsigned short utf8_to_euc_E8B2[];
	static const unsigned short utf8_to_euc_E8B3[];
	static const unsigned short utf8_to_euc_E8B4[];
	static const unsigned short utf8_to_euc_E8B5[];
	static const unsigned short utf8_to_euc_E8B6[];
	static const unsigned short utf8_to_euc_E8B7[];
	static const unsigned short utf8_to_euc_E8B8[];
	static const unsigned short utf8_to_euc_E8B9[];
	static const unsigned short utf8_to_euc_E8BA[];
	static const unsigned short utf8_to_euc_E8BB[];
	static const unsigned short utf8_to_euc_E8BC[];
	static const unsigned short utf8_to_euc_E8BD[];
	static const unsigned short utf8_to_euc_E8BE[];
	static const unsigned short utf8_to_euc_E8BF[];
	static const unsigned short utf8_to_euc_E980[];
	static const unsigned short utf8_to_euc_E981[];
	static const unsigned short utf8_to_euc_E982[];
	static const unsigned short utf8_to_euc_E983[];
	static const unsigned short utf8_to_euc_E984[];
	static const unsigned short utf8_to_euc_E985[];
	static const unsigned short utf8_to_euc_E986[];
	static const unsigned short utf8_to_euc_E987[];
	static const unsigned short utf8_to_euc_E988[];
	static const unsigned short utf8_to_euc_E989[];
	static const unsigned short utf8_to_euc_E98A[];
	static const unsigned short utf8_to_euc_E98B[];
	static const unsigned short utf8_to_euc_E98C[];
	static const unsigned short utf8_to_euc_E98D[];
	static const unsigned short utf8_to_euc_E98E[];
	static const unsigned short utf8_to_euc_E98F[];
	static const unsigned short utf8_to_euc_E990[];
	static const unsigned short utf8_to_euc_E991[];
	static const unsigned short utf8_to_euc_E992[];
	static const unsigned short utf8_to_euc_E995[];
	static const unsigned short utf8_to_euc_E996[];
	static const unsigned short utf8_to_euc_E997[];
	static const unsigned short utf8_to_euc_E998[];
	static const unsigned short utf8_to_euc_E999[];
	static const unsigned short utf8_to_euc_E99A[];
	static const unsigned short utf8_to_euc_E99B[];
	static const unsigned short utf8_to_euc_E99C[];
	static const unsigned short utf8_to_euc_E99D[];
	static const unsigned short utf8_to_euc_E99E[];
	static const unsigned short utf8_to_euc_E99F[];
	static const unsigned short utf8_to_euc_E9A0[];
	static const unsigned short utf8_to_euc_E9A1[];
	static const unsigned short utf8_to_euc_E9A2[];
	static const unsigned short utf8_to_euc_E9A3[];
	static const unsigned short utf8_to_euc_E9A4[];
	static const unsigned short utf8_to_euc_E9A5[];
	static const unsigned short utf8_to_euc_E9A6[];
	static const unsigned short utf8_to_euc_E9A7[];
	static const unsigned short utf8_to_euc_E9A8[];
	static const unsigned short utf8_to_euc_E9A9[];
	static const unsigned short utf8_to_euc_E9AA[];
	static const unsigned short utf8_to_euc_E9AB[];
	static const unsigned short utf8_to_euc_E9AC[];
	static const unsigned short utf8_to_euc_E9AD[];
	static const unsigned short utf8_to_euc_E9AE[];
	static const unsigned short utf8_to_euc_E9AF[];
	static const unsigned short utf8_to_euc_E9B0[];
	static const unsigned short utf8_to_euc_E9B1[];
	static const unsigned short utf8_to_euc_E9B3[];
	static const unsigned short utf8_to_euc_E9B4[];
	static const unsigned short utf8_to_euc_E9B5[];
	static const unsigned short utf8_to_euc_E9B6[];
	static const unsigned short utf8_to_euc_E9B7[];
	static const unsigned short utf8_to_euc_E9B8[];
	static const unsigned short utf8_to_euc_E9B9[];
	static const unsigned short utf8_to_euc_E9BA[];
	static const unsigned short utf8_to_euc_E9BB[];
	static const unsigned short utf8_to_euc_E9BC[];
	static const unsigned short utf8_to_euc_E9BD[];
	static const unsigned short utf8_to_euc_E9BE[];
	static const unsigned short utf8_to_euc_EFA4[];
	static const unsigned short utf8_to_euc_EFA7[];
	static const unsigned short utf8_to_euc_EFA8[];
	static const unsigned short utf8_to_euc_EFBC[];
	static const unsigned short utf8_to_euc_EFBD[];
	static const unsigned short utf8_to_euc_EFBD_ms[];
	static const unsigned short utf8_to_euc_EFBE[];
	static const unsigned short utf8_to_euc_EFBF[];
	static const unsigned short * const utf8_to_euc_E2[];
	static const unsigned short * const utf8_to_euc_E2_ms[];
	static const unsigned short * const utf8_to_euc_E2_932[];
	static const unsigned short * const utf8_to_euc_E2_mac[];
	static const unsigned short * const utf8_to_euc_E3[];
	static const unsigned short * const utf8_to_euc_E3_932[];
	static const unsigned short * const utf8_to_euc_E3_mac[];
	static const unsigned short * const utf8_to_euc_E4[];
	static const unsigned short * const utf8_to_euc_E5[];
	static const unsigned short * const utf8_to_euc_E6[];
	static const unsigned short * const utf8_to_euc_E7[];
	static const unsigned short * const utf8_to_euc_E8[];
	static const unsigned short * const utf8_to_euc_E9[];
	static const unsigned short * const utf8_to_euc_EF[];
	static const unsigned short * const utf8_to_euc_EF_ms[];
	static const unsigned short x0212_shiftjis_A2[];
	static const unsigned short x0212_shiftjis_B0[];
	static const unsigned short x0212_shiftjis_B1[];
	static const unsigned short x0212_shiftjis_B2[];
	static const unsigned short x0212_shiftjis_B3[];
	static const unsigned short x0212_shiftjis_B4[];
	static const unsigned short x0212_shiftjis_B5[];
	static const unsigned short x0212_shiftjis_B7[];
	static const unsigned short x0212_shiftjis_B8[];
	static const unsigned short x0212_shiftjis_B9[];
	static const unsigned short x0212_shiftjis_BA[];
	static const unsigned short x0212_shiftjis_BB[];
	static const unsigned short x0212_shiftjis_BC[];
	static const unsigned short x0212_shiftjis_BD[];
	static const unsigned short x0212_shiftjis_BE[];
	static const unsigned short x0212_shiftjis_BF[];
	static const unsigned short x0212_shiftjis_C0[];
	static const unsigned short x0212_shiftjis_C1[];
	static const unsigned short x0212_shiftjis_C2[];
	static const unsigned short x0212_shiftjis_C3[];
	static const unsigned short x0212_shiftjis_C4[];
	static const unsigned short x0212_shiftjis_C5[];
	static const unsigned short x0212_shiftjis_C6[];
	static const unsigned short x0212_shiftjis_C7[];
	static const unsigned short x0212_shiftjis_C8[];
	static const unsigned short x0212_shiftjis_C9[];
	static const unsigned short x0212_shiftjis_CA[];
	static const unsigned short x0212_shiftjis_CB[];
	static const unsigned short x0212_shiftjis_CC[];
	static const unsigned short x0212_shiftjis_CD[];
	static const unsigned short x0212_shiftjis_CE[];
	static const unsigned short x0212_shiftjis_CF[];
	static const unsigned short x0212_shiftjis_D0[];
	static const unsigned short x0212_shiftjis_D1[];
	static const unsigned short x0212_shiftjis_D2[];
	static const unsigned short x0212_shiftjis_D3[];
	static const unsigned short x0212_shiftjis_D4[];
	static const unsigned short x0212_shiftjis_D5[];
	static const unsigned short x0212_shiftjis_D7[];
	static const unsigned short x0212_shiftjis_D8[];
	static const unsigned short x0212_shiftjis_D9[];
	static const unsigned short x0212_shiftjis_DC[];
	static const unsigned short x0212_shiftjis_DD[];
	static const unsigned short x0212_shiftjis_DE[];
	static const unsigned short x0212_shiftjis_DF[];
	static const unsigned short x0212_shiftjis_E0[];
	static const unsigned short x0212_shiftjis_E1[];
	static const unsigned short x0212_shiftjis_E2[];
	static const unsigned short x0212_shiftjis_E3[];
	static const unsigned short x0212_shiftjis_E4[];
	static const unsigned short x0212_shiftjis_E5[];
	static const unsigned short x0212_shiftjis_E6[];
	static const unsigned short x0212_shiftjis_E7[];
	static const unsigned short x0212_shiftjis_E8[];
	static const unsigned short x0212_shiftjis_E9[];
	static const unsigned short x0212_shiftjis_EA[];
	static const unsigned short x0212_shiftjis_EB[];
	static const unsigned short x0212_shiftjis_EC[];
	static const unsigned short x0212_shiftjis_F3[];
	static const unsigned short x0212_shiftjis_F4[];
	/**
	 * UTF8_OUTPUT_ENABLE
	 */
	static const unsigned short euc_to_utf8_1byte[];
	static const unsigned short * const euc_to_utf8_2bytes[];
	static const unsigned short * const euc_to_utf8_2bytes_ms[];
	static const unsigned short * const euc_to_utf8_2bytes_mac[];
	static const unsigned short * const x0212_to_utf8_2bytes[];
	/**
	 * UTF8_INPUT_ENABLE
	 */
	static const unsigned short * const utf8_to_euc_2bytes[];
	static const unsigned short * const utf8_to_euc_2bytes_ms[];
	static const unsigned short * const utf8_to_euc_2bytes_932[];
	static const unsigned short * const utf8_to_euc_2bytes_mac[];
	static const unsigned short *const *const utf8_to_euc_3bytes[];
	static const unsigned short *const *const utf8_to_euc_3bytes_ms[];
	static const unsigned short *const *const utf8_to_euc_3bytes_932[];
	static const unsigned short *const *const utf8_to_euc_3bytes_mac[];
	/**
	 * SHIFTJIS_CP932
	 */
	static const unsigned short shiftjis_cp932[3][189];
	static const unsigned short cp932inv[2][189];
	/**
	 * X0212_ENABLE
	 */
	static const unsigned short shiftjis_x0212[3][189];
	static const unsigned short * const x0212_shiftjis[];

	static const struct normalization_pair normalization_table[];
};

#endif /* UTF8TABLE_H_ */
