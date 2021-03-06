/*
    NitroHax -- Cheat tool for the Nintendo DS
    Copyright (C) 2008  Michael "Chishm" Chisholm

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <nds.h>
#include <nds/arm7/input.h>
#include <nds/system.h>

#include "cheat_engine_arm7.h"


void VcountHandler() {
	inputGetAndSend();
}

void VblankHandler(void) {
}

unsigned int * ROMCTRL=(unsigned int*)0x40001A4; 
unsigned int * SCFG_EXT=(unsigned int*)0x4004008; 
unsigned int * SCFG_MC=(unsigned int*)0x4004010; 
unsigned int * SCFG_ROM=(unsigned int*)0x4004000;
unsigned int * SCFG_CLK=(unsigned int*)0x4004004; 

void PowerOffSlot()
{
	while(*SCFG_MC&0x0C ==  0x0C); 		// wait until state<>3
	if(*SCFG_MC&0x0C != 0x08) return; 		// exit if state<>2      
	
	*SCFG_MC = 0x0C;          		// set state=3 
	while(*SCFG_MC&0x0C !=  0x00);  // wait until state=0
}

void PowerOnSlot()
{
	while(*SCFG_MC&0x0C ==  0x0C); // wait until state<>3
	if(*SCFG_MC&0x0C != 0x00) return; //  exit if state<>0
	
	*SCFG_MC = 0x04;    // wait 1ms, then set state=1
	while(*SCFG_MC&0x0C != 0x04);
	
	*SCFG_MC = 0x08;    // wait 10ms, then set state=2      
	while(*SCFG_MC&0x0C != 0x08);
	
	*ROMCTRL = 0x20000000; // wait 27ms, then set ROMCTRL=20000000h
	
	while(*ROMCTRL&0x8000000 != 0x8000000);
}

void ResetSlot() {
	int backup =*SCFG_EXT;
	*SCFG_EXT=0xFFFFFFFF;	
	PowerOffSlot();
	PowerOnSlot();
	*SCFG_EXT=backup;
}

void runCardResetCheck(){
	if(*((vu32*)0x02111114) == (u32)0x1){
		// Card Reset Start here
		ResetSlot();
		
	*((vu32*)0x02111114) = (u32)0x0;
}
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	if(*SCFG_EXT == 0x92A00000) {
		*SCFG_EXT |= 0x830F0100; // NAND ACCESS
		// SCFG_CLK
		// 0x0180 : NTR
		// 0x0187 : TWL
		// 
		*SCFG_CLK |= 1;
	}
	
	
	irqInit();
	fifoInit();

	// read User Settings from firmware
	readUserSettings();

	// Start the RTC tracking IRQ
	initClockIRQ();

	SetYtrigger(80);

	installSystemFIFO();
	
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT);   

	// Keep the ARM7 mostly idle
	while (1) {
		runCardResetCheck();
		runCheatEngineCheck();
		swiWaitForVBlank();
	}
}

