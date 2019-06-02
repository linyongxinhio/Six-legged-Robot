#ifndef __FATTESTER_H
#define __FATTESTER_H 			     
#include "ff.h"
#include "sys.h"

 
u8 mf_mount(u8 drv);
u8 mf_open(u8*path,u8 mode);
u8 mf_close(void);
u8 mf_read(u16 len);
u8 mf_write(u8*dat,u16 len);
u8 mf_opendir(u8* path);
u8 mf_readdir(void);

u8 mf_scan_files(u8 * path);
u32 mf_showfree(u8 *drv);
u8 mf_lseek(u32 offset);
u32 mf_tell(void);
u32 mf_size(void);
u8 mf_mkdir(u8*name);
u8 mf_fmkfs(u8 drv,u8 mode,u16 au);
u8 mf_unlink(u8 *name);
u8 mf_rename(u8 *oldname,u8* newname);
void mf_gets(u16 size);
u8 mf_putc(u8 c);
u8 mf_puts(u8*c);

u8 mf_pfnmg_init(u8*path,u8 type);
u8 mf_pfnmg_get_pname(void);
u8 mf_pfnmg_get_plfname(void);
u8 mf_pfnmg_get_lfname(void);
u8 mf_pfnmg_get_ifname(u16 index);

u8 mf_cpymsg(u8*pname,u8 pct,u8 mode);
u32 mf_fdsize(u8 *fdname);//得到文件夹大小
u8 mf_fcopy(u8 *psrc,u8 *pdst);
u8 mf_fdcopy(u8 *psrc,u8 *pdst);
#endif





























