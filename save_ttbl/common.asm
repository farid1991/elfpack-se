
            RSEG CODE:CODE:NOROOT(2)
            public rightspatch
rightspatch:
            STMFD   SP!, {R1}
            ldr     r0,[sp,#4]
            bic     r1, r0, #0x1f
            orr     r1, r1, #0x13
            str     r1,[sp,#4]
            LDMFD   SP!, {R1}
            BX      LR            


            public callbios
callbios:
            STMFD   SP!, {R7,LR}
            MOV     R7, r1
            mov     r12, r0
            SVC     0xFE
            LDMFD   SP!, {R7,LR}
            BX      LR


            public biosInstall
            public biosOpen
            public biosUninstall
biosUninstall:
            MOV     R12, #3
            b driverfn_common
biosOpen:
            MOV     R12, #2
            b driverfn_common
biosInstall:
            MOV     R12, #1
driverfn_common:  
            MOV     R3, SP
            STR     LR, [SP,#-8]!
            SVC     0xFF
            LDR     LR, [SP],#8
            BX      LR

            public __copytbl
__copytbl:
            //r0 dest
            stmfd sp!,{r0-r7}       
        
            //disable irq
            mrs r7, CPSR
            orr r3, r7, #0xc0
            msr CPSR_c, r3
            //disable mmu
            mrc p15, 0, r5, c1, c0
            bic r3, r5, #0x1
            mcr p15, 0, r3, c1, c0
            nop
            nop
            nop
   
            mvn r6,#0
            MCR p15, 0, r6, c3, c0, 0

            mrc p15, 0, r1, c2,c0,0 //get tbl
            mov r2,#(16384/4)
l:        
            ldmia r1!,{r4}
            stmia r0!,{r4}
            subs r2,r2,#1
            bne l
        
            //restore mmu
            mcr p15, 0, r5, c1, c0
            nop
            nop
            nop
        
            //restore irq
            msr CPSR_c, r7
        
            ldmfd sp!,{r0-r7}
            bx lr
      




            public __copydata
__copydata:
            //r0 dest r1 src r2 len/4
            stmfd sp!,{r0-r7}       
        
            //disable irq
            mrs r7, CPSR
            orr r3, r7, #0xc0
            msr CPSR_c, r3
            //disable mmu
            mrc p15, 0, r5, c1, c0
            bic r3, r5, #0x1
            mcr p15, 0, r3, c1, c0
            nop
            nop
            nop
   
            mvn r6,#0
            MCR p15, 0, r6, c3, c0, 0

;            mrc p15, 0, r1, c2,c0,0 //get tbl
;            mov r2,#(16384/4)
l3:        
            ldmia r1!,{r4}
            stmia r0!,{r4}
            subs r2,r2,#1
            bne l3
        
            //restore mmu
            mcr p15, 0, r5, c1, c0
            nop
            nop
            nop
        
            //restore irq
            msr CPSR_c, r7
        
            ldmfd sp!,{r0-r7}
            bx lr
      




  







            public __realaddr2
__realaddr2:
            //r0 addr
            stmfd sp!,{r1-r6}       
        
            //disable irq
            mrs r2, CPSR
            orr r3, r2, #0xc0
            msr CPSR_c, r3        
            //disable mmu
            mrc p15, 0, r5, c1, c0
            bic r3, r5, #0x1
            mcr p15, 0, r3, c1, c0
            nop
            nop
            nop
   
            mvn r6,#0
            MCR p15, 0, r6, c3, c0, 0

            mrc p15, 0, r1, c2,c0,0 //get tbl

            ;r0 - addr
            ;r1 - ttbl
            ;r2 - irq
            ;r5 - mmu state
			
			;free - r3,r4,r6

			mov r6,#0xFFFFFFFF	//	unsigned long mask = 0xFFFFFFFF;

			;free - r3,r4

			lsr r3,r0,#20	//addr/0x100000
			ldr r1,[r1, r3,lsl #2] //value = firstlevel[ (unsigned)addr/0x100000 ];

			;free - r3,r4
			
			and r3,r1,#3
//	switch(value & 3)
			cmp r3,#2
			bne ral2
//	case 2:
//		//section
			lsr r6,r6,#12 //mask = 0xFFFFF;
			b ral1
ral2:			
			cmp r3,#1
			bne ral1
//	case 1:
//		//coarse

			lsr r1,r1,#10
			lsl r1,r1,#10 //level2table = (unsigned long*) (value & ~0x3FF);
		
			lsr r3,r0,#12
			and r3,r3,#0xFF
		
			ldr r1,[r1, r3, lsl #2] //value = level2table[ ((unsigned long)addr >>12) & 0xFF ];

			and r3, r1, #3
			//switch(value & 3)
			cmp r3, #1
			bne ral3
			//case 1:
			lsr r6,r6,#16//mask = 0xffff;
			b ral1
ral3:		
			cmp r3,#2
			bne ral4
			//case 2:
			lsr r6,r6,#20//			mask = 0xfff;
			b ral1
ral4:
			cmp r3,#3
			bne ral1
			//case 3:
			lsr r6,r6,#22//			mask = 0x3ff;


ral1:
//	return (value & ~mask) | ((long)addr & mask);
			and r0, r0, r6

			mvn r3, r6
			and r1, r1, r3
		
			orr r0, r0,r1
		

            //restore mmu
            mcr p15, 0, r5, c1, c0
            nop
            nop
            nop
        
            //restore irq
            msr CPSR_c, r2
        
            ldmfd sp!,{r1-r6}

            bx lr






















            public __getdword
__getdword:
            //r0 addr
            stmfd sp!,{r1-r7}       
        
            //disable irq
            mrs r7, CPSR
            orr r3, r7, #0xc0
            msr CPSR_c, r3
            //disable mmu
            mrc p15, 0, r5, c1, c0
            bic r3, r5, #0x1
            mcr p15, 0, r3, c1, c0
            nop
            nop
            nop
   
            mvn r6,#0
            MCR p15, 0, r6, c3, c0, 0

	ldr r0,[r0]


        
            //restore mmu
            mcr p15, 0, r5, c1, c0
            nop
            nop
            nop
        
            //restore irq
            msr CPSR_c, r7
        
            ldmfd sp!,{r1-r7}
            bx lr

            public __someend
__someend:


			end
