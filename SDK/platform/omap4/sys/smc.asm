;******************************************************************************

;****************************** Global Symbols*********************************
        .global omap_smc_rom
        .global omap_modify_auxcoreboot0
        .global omap_auxcoreboot_addr
        .global omap_read_auxcoreboot0

;**************************** Text Section ************************************
        .text

; This source file is assembled for ARM instructions
        .state32

;*****************************************************************************
omap_smc_rom:
    push {r4-r11,lr}
    mov    r12,    r0
    mov    r0,    r1
    dsb
    isb
    dmb
    smi      #0
    pop {r4-r11,pc}


omap_modify_auxcoreboot0:
	push {r1-r12, lr}
    mov     r12, #0x104
	dsb
	;smc	#0
	pop {r1-r12, pc}


omap_auxcoreboot_addr:
	push {r2-r12, lr}
    mov     r12, #0x105
	dsb
	;smc	#0
	pop {r2-r12, pc}


omap_read_auxcoreboot0:
	push {r2-r12, lr}
    mov     r12, #0x103
	dsb
	;smc	#0
	mov	r0, r0, lsr #9
	pop {r2-r12, pc}

;
; End of the file
;
    .end
