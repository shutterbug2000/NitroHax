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

void runCheatEngineCheck (void)
{
	if(*((vu32*)0x027FFE24) == (u32)0x027FFE04)
	{
		irqDisable (IRQ_ALL);
		*((vu32*)0x027FFE34) = (u32)0x06000000;
		
		// if dsi mode is detected try switch back to ntr mode
		// this may require extended access
		// from http://problemkaputt.de/gbatek.htm#dsinotes
		// if ([4004000h] AND 03h)=01h then DSi_mode else NDS_mode
		// Caution: Below detection won't work with DSi exploits (because they are
		// usually having the ARM7 SCFG registers disabled - it would be thus better
		// to do the dection only on ARM9 side as described above, and then forward
		// the result to ARM7 side).
		// if ([4004008h] AND 80000000h)=0 then skip_detection_and_assume_NDS_mode
		// else if ([4004000h] AND 03h)=01h then DSi_mode else NDS_mode
		
		unsigned int * SCFG_ROM=	(unsigned int*)0x4004000;
		unsigned int * SCFG_EXT=	(unsigned int*)0x4004008;
		// Disabled. No longer needed now that we got bit31 patched out. :D
		
		//if(*SCFG_EXT & 0x80000000 != 0)  {
			//if (*SCFG_ROM & 0x03==0x01) {
				//*SCFG_ROM = 0;
			//}
		//}
			
		swiSoftReset();
	} 
}
