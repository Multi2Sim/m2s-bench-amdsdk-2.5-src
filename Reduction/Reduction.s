.global reduce

.metadata
	
	COMPUTE_PGM_RSRC2:USER_SGPR      = 12
	COMPUTE_PGM_RSRC2:TGID_X_EN      = 1
	COMPUTE_PGM_RSRC2:LDS_SIZE       = 128
	
	userElements[0]    = PTR_UAV_TABLE, 0, s[2:3]
	userElements[1]    = IMM_CONST_BUFFER, 0, s[4:7]
	userElements[2]    = IMM_CONST_BUFFER, 1, s[8:11]

	FloatMode            = 192
	IeeeMode             = 0
	rat_op = 0x0c00



.args

	u32[4]* input 0 uav10 RO
	u32[4]* output 16 uav11 RW
	u32[4]* sdata 32 hl RW


.text

	s_mov_b32     m0, 0x00008000                              // 00000000: BEFC03FF 00008000
	s_buffer_load_dword  s0, s[4:7], 0x04                     // 00000008: C2000504
	s_buffer_load_dword  s1, s[4:7], 0x18                     // 0000000C: C2008518
	s_buffer_load_dword  s4, s[4:7], 0x1c                     // 00000010: C202051C
	s_buffer_load_dword  s5, s[8:11], 0x00                    // 00000014: C2028900
	s_buffer_load_dword  s6, s[8:11], 0x04                    // 00000018: C2030904
	s_buffer_load_dword  s7, s[8:11], 0x08                    // 0000001C: C2038908
	s_load_dwordx4  s[8:11], s[2:3], 0x50                     // 00000020: C0840350
	s_waitcnt     lgkmcnt(0)                                  // 00000024: BF8C007F
	s_min_u32     s13, s0, 0x0000ffff                         // 00000028: 838DFF00 0000FFFF
	v_mov_b32     v1, s13                                     // 00000030: 7E02020D
	v_mul_i32_i24  v1, s12, v1                                // 00000034: 1202020C
	v_add_i32     v1, vcc, v0, v1                             // 00000038: 4A020300
	v_add_i32     v1, vcc, s1, v1                             // 0000003C: 4A020201
	s_add_i32     s1, s12, s4                                 // 00000040: 8101040C
	v_lshlrev_b32  v2, 4, v0                                  // 00000044: 34040084
	v_add_i32     v2, vcc, s7, v2                             // 00000048: 4A040407
	v_lshlrev_b32  v1, 4, v1                                  // 0000004C: 34020284
	v_add_i32     v1, vcc, s5, v1                             // 00000050: 4A020205
	v_add_i32     v3, vcc, 4, v2                              // 00000054: 4A060484
	v_add_i32     v4, vcc, 8, v2                              // 00000058: 4A080488
	v_add_i32     v5, vcc, 12, v2                             // 0000005C: 4A0A048C
	tbuffer_load_format_xyzw  v[6:9], v1, s[8:11], 0 offen format:[BUF_DATA_FORMAT_32_32_32_32,BUF_NUM_FORMAT_FLOAT] // 00000060: EBF31000 80020601
	s_waitcnt     vmcnt(0)                                    // 00000068: BF8C1F70
	ds_write_b32  v2, v6                                      // 0000006C: D8340000 00000602
	ds_write_b32  v3, v7                                      // 00000074: D8340000 00000703
	ds_write_b32  v4, v8                                      // 0000007C: D8340000 00000804
	ds_write_b32  v5, v9                                      // 00000084: D8340000 00000905
	s_waitcnt     lgkmcnt(0)                                  // 0000008C: BF8C007F
	s_barrier                                                 // 00000090: BF8A0000
	s_lshr_b32    s0, s0, 1                                   // 00000094: 90008100
	s_cmp_eq_i32  s0, 0                                       // 00000098: BF008000
	s_cbranch_scc1  label_0058                                // 0000009C: BF850030
	label_0028:
	v_cmp_gt_u32  s[4:5], s0, v0                              // 000000A0: D1880004 00020000
	s_and_saveexec_b64  s[4:5], s[4:5]                        // 000000A8: BE842404
	v_add_i32     v1, vcc, s0, v0                             // 000000AC: 4A020000
	s_cbranch_execz  label_0052                               // 000000B0: BF880025
	v_lshlrev_b32  v1, 4, v1                                  // 000000B4: 34020284
	v_add_i32     v1, vcc, s7, v1                             // 000000B8: 4A020207
	v_add_i32     v6, vcc, 4, v1                              // 000000BC: 4A0C0284
	v_add_i32     v7, vcc, 8, v1                              // 000000C0: 4A0E0288
	v_add_i32     v8, vcc, 12, v1                             // 000000C4: 4A10028C
	ds_read_b32   v9, v2                                      // 000000C8: D8D80000 09000002
	ds_read_b32   v1, v1                                      // 000000D0: D8D80000 01000001
	ds_read_b32   v10, v3                                     // 000000D8: D8D80000 0A000003
	ds_read_b32   v11, v4                                     // 000000E0: D8D80000 0B000004
	ds_read_b32   v12, v5                                     // 000000E8: D8D80000 0C000005
	ds_read_b32   v6, v6                                      // 000000F0: D8D80000 06000006
	ds_read_b32   v7, v7                                      // 000000F8: D8D80000 07000007
	ds_read_b32   v8, v8                                      // 00000100: D8D80000 08000008
	s_waitcnt     lgkmcnt(6)                                  // 00000108: BF8C067F
	v_add_i32     v1, vcc, v9, v1                             // 0000010C: 4A020309
	s_waitcnt     lgkmcnt(2)                                  // 00000110: BF8C027F
	v_add_i32     v6, vcc, v10, v6                            // 00000114: 4A0C0D0A
	ds_write_b32  v2, v1                                      // 00000118: D8340000 00000102
	s_waitcnt     lgkmcnt(2)                                  // 00000120: BF8C027F
	v_add_i32     v1, vcc, v11, v7                            // 00000124: 4A020F0B
	ds_write_b32  v3, v6                                      // 00000128: D8340000 00000603
	s_waitcnt     lgkmcnt(2)                                  // 00000130: BF8C027F
	v_add_i32     v6, vcc, v12, v8                            // 00000134: 4A0C110C
	ds_write_b32  v4, v1                                      // 00000138: D8340000 00000104
	ds_write_b32  v5, v6                                      // 00000140: D8340000 00000605
	label_0052:
	s_mov_b64     exec, s[4:5]                                // 00000148: BEFE0404
	s_waitcnt     lgkmcnt(0)                                  // 0000014C: BF8C007F
	s_barrier                                                 // 00000150: BF8A0000
	s_lshr_b32    s0, s0, 1                                   // 00000154: 90008100
	s_cmp_eq_i32  s0, 0                                       // 00000158: BF008000
	s_cbranch_scc0  label_0028                                // 0000015C: BF84FFD0
	label_0058:
	v_cmp_eq_i32  s[4:5], v0, 0                               // 00000160: D1040004 00010100
	s_and_saveexec_b64  s[4:5], s[4:5]                        // 00000168: BE842404
	s_cbranch_execz  label_0072                               // 0000016C: BF880016
	s_add_i32     s0, s7, 4                                   // 00000170: 81008407
	s_add_i32     s8, s7, 8                                   // 00000174: 81088807
	s_add_i32     s9, s7, 12                                  // 00000178: 81098C07
	v_mov_b32     v0, s7                                      // 0000017C: 7E000207
	v_mov_b32     v1, s0                                      // 00000180: 7E020200
	v_mov_b32     v2, s8                                      // 00000184: 7E040208
	v_mov_b32     v3, s9                                      // 00000188: 7E060209
	ds_read_b32   v0, v0                                      // 0000018C: D8D80000 00000000
	ds_read_b32   v1, v1                                      // 00000194: D8D80000 01000001
	ds_read_b32   v2, v2                                      // 0000019C: D8D80000 02000002
	ds_read_b32   v3, v3                                      // 000001A4: D8D80000 03000003
	s_load_dwordx4  s[8:11], s[2:3], 0x58                     // 000001AC: C0840358
	s_lshl_b32    s0, s1, 4                                   // 000001B0: 8F008401
	s_add_i32     s0, s6, s0                                  // 000001B4: 81000006
	v_mov_b32     v4, s0                                      // 000001B8: 7E080200
	s_waitcnt     lgkmcnt(0)                                  // 000001BC: BF8C007F
	tbuffer_store_format_xyzw  v[0:3], v4, s[8:11], 0 offen format:[BUF_DATA_FORMAT_32_32_32_32,BUF_NUM_FORMAT_FLOAT] // 000001C0: EBF71000 80020004
	label_0072:
	s_mov_b64     exec, s[4:5]                                // 000001C8: BEFE0404
	s_endpgm                                                  // 000001CC: BF810000
