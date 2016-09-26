.text
.code64
.global arrayHandlers

#define commonHandler(n) \
handler##n: \
	pushq $0; \
	pushq $n; \
	jmp cHandle \


#define HandlerWithErrCode(n) \
handler##n: \
	pushq $n; \
	jmp cHandle \

commonHandler(0)
commonHandler(1)
commonHandler(2)
commonHandler(3)
commonHandler(4)
commonHandler(5)
commonHandler(6)
commonHandler(7)
HandlerWithErrCode(8)
commonHandler(9)
HandlerWithErrCode(10)
HandlerWithErrCode(11)
HandlerWithErrCode(12)
HandlerWithErrCode(13)
HandlerWithErrCode(14)
commonHandler(15)
commonHandler(16)
HandlerWithErrCode(17)
commonHandler(18)
commonHandler(19)
commonHandler(20)
commonHandler(21)
commonHandler(22)
commonHandler(23)
commonHandler(24)
commonHandler(25)
commonHandler(26)
commonHandler(27)
commonHandler(28)
commonHandler(29)
commonHandler(30)
commonHandler(31)
commonHandler(32)
commonHandler(33)
commonHandler(34)
commonHandler(35)
commonHandler(36)
commonHandler(37)
commonHandler(38)
commonHandler(39)
commonHandler(40)
commonHandler(41)
commonHandler(42)
commonHandler(43)
commonHandler(44)
commonHandler(45)
commonHandler(46)
commonHandler(47)
commonHandler(48)
commonHandler(49)
commonHandler(50)
commonHandler(51)
commonHandler(52)
commonHandler(53)
commonHandler(54)
commonHandler(55)
commonHandler(56)
commonHandler(57)
commonHandler(58)
commonHandler(59)
commonHandler(60)
commonHandler(61)
commonHandler(62)
commonHandler(63)
commonHandler(64)
commonHandler(65)
commonHandler(66)
commonHandler(67)
commonHandler(68)
commonHandler(69)
commonHandler(70)
commonHandler(71)
commonHandler(72)
commonHandler(73)
commonHandler(74)
commonHandler(75)
commonHandler(76)
commonHandler(77)
commonHandler(78)
commonHandler(79)
commonHandler(80)
commonHandler(81)
commonHandler(82)
commonHandler(83)
commonHandler(84)
commonHandler(85)
commonHandler(86)
commonHandler(87)
commonHandler(88)
commonHandler(89)
commonHandler(90)
commonHandler(91)
commonHandler(92)
commonHandler(93)
commonHandler(94)
commonHandler(95)
commonHandler(96)
commonHandler(97)
commonHandler(98)
commonHandler(99)

arrayHandlers:
	.quad handler0
	.quad handler1
	.quad handler2
	.quad handler3
	.quad handler4
	.quad handler5
	.quad handler6
	.quad handler7
	.quad handler8
	.quad handler9
	.quad handler10
	.quad handler11
    	.quad handler12
	.quad handler13
	.quad handler14
	.quad handler15
	.quad handler16
	.quad handler17
	.quad handler18
	.quad handler19
	.quad handler20
	.quad handler21
	.quad handler22
	.quad handler23
	.quad handler24
	.quad handler25
	.quad handler26
	.quad handler27
	.quad handler28
	.quad handler29
	.quad handler30
	.quad handler31
	.quad handler32
	.quad handler33
	.quad handler34
	.quad handler35
    	.quad handler36
	.quad handler37
	.quad handler38
	.quad handler39
	.quad handler40
	.quad handler41
	.quad handler42
	.quad handler43
	.quad handler44
	.quad handler45
	.quad handler46
	.quad handler47
	.quad handler48
	.quad handler49
	.quad handler50
	.quad handler51
	.quad handler52
	.quad handler53
	.quad handler54
	.quad handler55
	.quad handler56
	.quad handler57
	.quad handler58
	.quad handler59
    	.quad handler60
	.quad handler61
	.quad handler62
	.quad handler63
	.quad handler64
	.quad handler65
	.quad handler66
	.quad handler67
	.quad handler68
	.quad handler69
	.quad handler70
	.quad handler71
	.quad handler72
	.quad handler73
	.quad handler74
	.quad handler75
	.quad handler76
	.quad handler77
	.quad handler78
	.quad handler79
	.quad handler80
	.quad handler81
	.quad handler82
	.quad handler83
    	.quad handler84
	.quad handler85
	.quad handler86
	.quad handler87
	.quad handler88
	.quad handler89
	.quad handler90
	.quad handler91
	.quad handler92
	.quad handler93
	.quad handler94
	.quad handler95
	.quad handler96
	.quad handler97
	.quad handler98
	.quad handler99

cHandle:
	pushq %rdi
	movq 8(%rsp), %rdi 

	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rsi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

    cld
	call interruptHandler

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rsi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	popq %rdi

	addq $16, %rsp
	iretq
