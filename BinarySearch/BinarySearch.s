.global binarySearch


.metadata
	
	userElements[0] = PTR_UAV_TABLE, 0, s[2:3]
	userElements[1] = IMM_CONST_BUFFER, 0, s[4:7]
	userElements[2] = IMM_CONST_BUFFER, 1, s[8:11]
	FloatMode = 192
	IeeeMode = 0
	
	
	rat_op = 0x0c00
	COMPUTE_PGM_RSRC2:USER_SGPR = 12
	COMPUTE_PGM_RSRC2:TGID_X_EN = 1

.args
	
	
	u32[4]* outputArray 0 uav10 RW
	u32* sortedArray 16 uav11 const RO


	u32 findMe 32
	u32 globalLowerBound 48
	u32 globalUpperBound 64
	u32 subdivSize 80

.text


  s_buffer_load_dword  s0, s[4:7], 0x04                     // 00000000: C2000504
  s_buffer_load_dword  s1, s[4:7], 0x18                     // 00000004: C2008518
  s_waitcnt     lgkmcnt(0)                                  // 00000008: BF8C007F
  s_min_u32     s0, s0, 0x0000ffff                          // 0000000C: 8380FF00 0000FFFF
  s_buffer_load_dword  s4, s[8:11], 0x14                    // 00000014: C2020914
  v_mov_b32     v1, s0                                      // 00000018: 7E020200
  s_buffer_load_dword  s0, s[8:11], 0x0c                    // 0000001C: C200090C
  v_mul_i32_i24  v1, s12, v1                                // 00000020: D2120001 0002020C
  v_add_i32     v0, vcc, v0, v1                             // 00000028: 4A000300
  s_buffer_load_dword  s5, s[8:11], 0x04                    // 0000002C: C2028904
  v_add_i32     v0, vcc, s1, v0                             // 00000030: 4A000001
  s_waitcnt     lgkmcnt(0)                                  // 00000034: BF8C007F
  v_mul_lo_i32  v0, v0, s4                                  // 00000038: D2D60000 00000900
  v_add_i32     v3, vcc, s0, v0                             // 00000040: 4A060000
  v_lshlrev_b32  v1, 2, v3                                  // 00000044: 34020682
  v_add_i32     v1, vcc, s5, v1                             // 00000048: 4A020205
  s_load_dwordx4  s[12:15], s[2:3], 0x58                    // 0000004C: C0860358
  s_waitcnt     lgkmcnt(0)                                  // 00000050: BF8C007F
  tbuffer_load_format_x  v1, v1, s[12:15], 0 offen format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 00000054: EBA01000 80030101
  s_buffer_load_dword  s0, s[8:11], 0x08                    // 0000005C: C2000908
  s_buffer_load_dword  s1, s[8:11], 0x00                    // 00000060: C2008900
  s_waitcnt     vmcnt(0) & lgkmcnt(0)                       // 00000064: BF8C0070
  v_cmp_ge_u32  s[6:7], s0, v1                              // 00000068: D18C0006 00020200
  s_and_saveexec_b64  s[6:7], s[6:7]                        // 00000070: BE862406
  v_add_i32     v1, vcc, s4, v3                             // 00000074: 4A020604
  s_cbranch_execz  label_0033                               // 00000078: BF880014
  v_add_i32     v4, vcc, -1, v1                             // 0000007C: 4A0802C1
  v_lshlrev_b32  v2, 2, v4                                  // 00000080: 34040882
  v_add_i32     v2, vcc, s5, v2                             // 00000084: 4A040405
  tbuffer_load_format_x  v2, v2, s[12:15], 0 offen format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 00000088: EBA01000 80030202
  s_waitcnt     vmcnt(0)                                    // 00000090: BF8C1F70
  v_cmp_gt_u32  s[4:5], s0, v2                              // 00000094: D1880004 00020400
  s_mov_b64     vcc, exec                                   // 0000009C: BEEA047E
  s_andn2_b64   exec, vcc, s[4:5]                           // 000000A0: 8AFE046A
  s_cbranch_execz  label_0033                               // 000000A4: BF880009
  s_load_dwordx4  s[8:11], s[2:3], 0x50                     // 000000A8: C0840350
  v_mov_b32     v2, s1                                      // 000000AC: 7E040201
  s_waitcnt     lgkmcnt(0)                                  // 000000B0: BF8C007F
  tbuffer_load_format_x  v5, v2, s[8:11], 0 offen offset:8 format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 000000B4: EBA01008 80020502
  v_mov_b32     v6, 1                                       // 000000BC: 7E0C0281
  s_waitcnt     vmcnt(0)                                    // 000000C0: BF8C1F70
  tbuffer_store_format_xyzw  v[3:6], v2, s[8:11], 0 offen format:[BUF_DATA_FORMAT_32_32_32_32,BUF_NUM_FORMAT_FLOAT] // 000000C4: EBF71000 80020302
label_0033:
  s_mov_b64     exec, s[6:7]                                // 000000CC: BEFE0406
  s_endpgm                                                  // 000000D0: BF810000



