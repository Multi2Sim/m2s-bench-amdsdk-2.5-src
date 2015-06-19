.global bitonicSort

.mem

.data
	FloatMode = 192
	IeeeMode = 0

	userElements[0] = IMM_UAV, 10, s[4:7]
	userElements[1] = IMM_CONST_BUFFER, 0, s[8:11]
	userElements[2] = IMM_CONST_BUFFER, 1, s[12:15]
	
	COMPUTE_PGM_RSRC2:USER_SGPR = 16
	COMPUTE_PGM_RSRC2:TGID_X_EN = 1
.arg
	
	u32* theArray 0 uav10 RW
	u32 value:stage 16
	u32 passOfStage 32
	u32 width 48
	u32 direction 64

.text
  
  s_buffer_load_dword  s0, s[8:11], 0x04                    // 00000000: C2000904
  s_buffer_load_dword  s1, s[8:11], 0x18                    // 00000004: C2008918
  s_buffer_load_dword  s2, s[12:15], 0x04                   // 00000008: C2010D04
  s_buffer_load_dword  s3, s[12:15], 0x08                   // 0000000C: C2018D08
  s_waitcnt     lgkmcnt(0)                                  // 00000010: BF8C007F
  s_min_u32     s0, s0, 0x0000ffff                          // 00000014: 8380FF00 0000FFFF
  v_mov_b32     v1, s0                                      // 0000001C: 7E020200
  v_mul_i32_i24  v1, s16, v1                                // 00000020: 12020210
  v_add_i32     v0, vcc, v0, v1                             // 00000024: 4A000300
  v_add_i32     v0, vcc, s1, v0                             // 00000028: 4A000001
  s_sub_i32     s0, s2, s3                                  // 0000002C: 81800302
  v_lshrrev_b32  v1, s0, v0                                 // 00000030: 2C020000
  s_lshl_b32    s1, 1, s0                                   // 00000034: 8F010081
  s_buffer_load_dword  s3, s[12:15], 0x00                   // 00000038: C2018D00
  v_mul_lo_i32  v1, s1, v1                                  // 0000003C: D2D60001 02020201
  v_lshlrev_b32  v1, 1, v1                                  // 00000044: 34020281
  v_bfe_u32     v2, v0, 0, s0                               // 00000048: D2900002 00010100
  v_add_i32     v1, vcc, v1, v2                             // 00000050: 4A020501
  v_add_i32     v2, vcc, s1, v1                             // 00000054: 4A040201
  v_lshlrev_b32  v1, 2, v1                                  // 00000058: 34020282
  v_lshlrev_b32  v2, 2, v2                                  // 0000005C: 34040482
  s_waitcnt     lgkmcnt(0)                                  // 00000060: BF8C007F
  v_add_i32     v1, vcc, s3, v1                             // 00000064: 4A020203
  v_add_i32     v2, vcc, s3, v2                             // 00000068: 4A040403
  tbuffer_load_format_x  v3, v1, s[4:7], 0 offen format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 0000006C: EBA01000 80010301
  tbuffer_load_format_x  v4, v2, s[4:7], 0 offen format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 00000074: EBA01000 80010402
  s_buffer_load_dword  s0, s[12:15], 0x10                   // 0000007C: C2000D10
  s_lshl_b32    s1, 1, s2                                   // 00000080: 8F010281
  s_waitcnt     lgkmcnt(0)                                  // 00000084: BF8C007F
  s_sub_i32     s2, 1, s0                                   // 00000088: 81820081
  v_and_b32     v0, s1, v0                                  // 0000008C: 36000001
  v_cmp_eq_i32  vcc, 0, v0                                  // 00000090: 7D040080
  v_mov_b32     v0, s0                                      // 00000094: 7E000200
  v_mov_b32     v5, s2                                      // 00000098: 7E0A0202
  v_cndmask_b32  v0, v5, v0, vcc                            // 0000009C: 00000105
  s_waitcnt     vmcnt(0)                                    // 000000A0: BF8C1F70
  v_min_u32     v5, v3, v4                                  // 000000A4: 260A0903
  v_max_u32     v3, v3, v4                                  // 000000A8: 28060903
  v_cmp_eq_i32  vcc, 0, v0                                  // 000000AC: 7D040080
  v_cndmask_b32  v0, v5, v3, vcc                            // 000000B0: 00000705
  tbuffer_store_format_x  v0, v1, s[4:7], 0 offen format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 000000B4: EBA41000 80010001
  s_waitcnt     expcnt(0)                                   // 000000BC: BF8C1F0F
  v_cndmask_b32  v0, v3, v5, vcc                            // 000000C0: 00000B03
  tbuffer_store_format_x  v0, v2, s[4:7], 0 offen format:[BUF_DATA_FORMAT_32,BUF_NUM_FORMAT_FLOAT] // 000000C4: EBA41000 80010002
  s_endpgm                                                  // 000000CC: BF810000
