#include<elf.h>
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<memory/paddr.h>
typedef struct{
	char* name;
	paddr_t start;
	uint32_t size;
}Elf32_Func;

int fn_size = 0;
Elf32_Func* fn_table = NULL;
void init_ftracer(const char* target){
	// 获取文件指针并且判断类型
	FILE*fp = fopen("add-riscv32-nemu.elf","rb");
	Assert(fp!=NULL,"error file name");
	Elf32_Ehdr elf_header;
	fread(&elf_header,sizeof(Elf32_Ehdr),1,fp);
	Assert(elf_header.e_machine==EM_RISCV,"error file type");
	Assert(elf_header.e_type==ET_EXEC,"error file type");

	fseek(fp,elf_header.e_shoff,SEEK_SET);
	Elf32_Shdr* sh_table = (Elf32_Shdr*)malloc(sizeof(Elf32_Shdr)*elf_header.e_shnum);
	fread(sh_table,sizeof(Elf32_Shdr),elf_header.e_shnum,fp);
	// 找到strtab节
	char name_buf[256];
	int strndx;
	for(int i=1;i<elf_header.e_shnum;i++){
		if(sh_table[i].sh_type==SHT_STRTAB){
			fseek(fp,sh_table[elf_header.e_shstrndx].sh_offset+sh_table[i].sh_name,SEEK_SET);
			fgets(name_buf,255,fp);
			if(strcmp(name_buf,".strtab")==0){
				strndx = i;
				break;
			}
		}
	}
	for(int i=1;i<elf_header.e_shnum;i++){
		// 找到符号表
		if(sh_table[i].sh_type==SHT_SYMTAB){
			Elf32_Sym* sym_table = (Elf32_Sym*)malloc(sh_table[i].sh_size);
			fseek(fp,sh_table[i].sh_offset,SEEK_SET);
			int count = sh_table[i].sh_size/sizeof(Elf32_Sym);
			fread(sym_table,sizeof(Elf32_Sym),count,fp);
            // 找到符号表里面的函数，并计数
			for(int j=0;j<count;j++){
				if((sym_table[j].st_info&0x0f)==STT_FUNC){
					fn_size++;
				}
			}
	        Assert(fn_size!=0,"no func found");
            // 动态分配fn_table
			fn_table = (Elf32_Func*)malloc(sizeof(Elf32_Func)*fn_size);
	        Assert(fn_table!=0,"fn_table allocate error");
            // 写入fn_table
			for(int j=0,k=0;j<count;j++){
				if((sym_table[j].st_info&0x0f)==STT_FUNC){
					fn_table[k].start = sym_table[j].st_value;
					fn_table[k].size = sym_table[j].st_size;
                    // 需要在strtab中找到对应的名字，用以可视化
					fseek(fp,sh_table[strndx].sh_offset+sym_table[j].st_name,SEEK_SET);
					fgets(name_buf,255,fp);
					fn_table[k].name = (char*)malloc(sizeof(char)*strlen(name_buf)+1);
					strcpy(fn_table[k].name,name_buf);
					k++;
				}
			}
			break;
		}
	}
	fclose(fp);
}
