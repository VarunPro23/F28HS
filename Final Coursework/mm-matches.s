@ Used the CPUlator to run and debug your code: https://cpulator.01xz.net/?sys=arm-de1soc&d_audio=48000
@ Note, that the CPUlator simulates a DE1-SoC device, and here you should use the HEX displays to show the numbers
@ See the Tutorials on LED, Button, and HEX displays in the F28HS course (Weeks 8 and 9)

@ This ARM Assembler code should implement a matching function, for use in MasterMind program, as
@ described in the CW3 specification. It should produce as output 2 numbers, the first for the
@ exact matches (peg of right colour and in right position) and approximate matches (peg of right
@ color but not in right position). Make sure to count each peg just once!
	
@ Example (first sequence is secret, second sequence is guess):
@ 1 2 1
@ 3 1 3 ==> 0 1
@ Display the result as two digits on the two rightmost HEX displays, from left to right
@

@ -----------------------------------------------------------------------------

.text
.global         main
main: 
	LDR  R2, =secret	@ pointer to secret sequence
	LDR  R3, =guess		@ pointer to guess sequence

	@ you probably need to initialise more values here

	@ ... COMPLETE THE CODING BY ADDING YOUR CODE HERE, you may want to use sub-routines to structure your code
	                    @ prepare registers for 'match' subroutine
	MOV  R0, R2         @ pass pointer to secret sequence as argument
	MOV  R1, R3         @ pass pointer to guess sequence as argument
	PUSH {R2, R3}
	BL   match          @ branch to 'match' subroutine
	BL   display        @ branch to 'display' subroutine
	POP  {R2, R3}
	B    exit


match:
	@ Counts the exact and approximate matches in the guess sequence
	@ Register usage: 
	@ R0  - pointer to secret sequence (argument)
	@ R1  - pointer to guess sequence (argument)
	@ R4  - exact matches counter
	@ R5  - approx. matches counter
	@ R6  - array index for secret sequence
	@ R7  - array index for guess sequence
	@ R8  - digit of secret sequence
	@ R10 - digit of guess sequence
	@
	@ Returns:
	@ R0  - number of exact matches
	@ R1  - number of approximate matches
	@
	@ References:
	@ [1] Memory Instructions: Load and Store, Azeria Labs (Online Article)
	@     Available at: https://azeria-labs.com/memory-instructions-load-and-store-part-4/
	
	PUSH {R4-R8, R10}           @ preserve contents of registers required for this subroutine
	
	MOV  R4, #0                 @ set initial exact and approx. match count to zero     
	MOV  R5, #0
	MOV  R6, #0                 @ set starting array indices to zero
	MOV  R7, #0

exact:
	LDR  R8, [R0, R6, LSL #2]   @ get the digit of the secret sequence at the current index [1]
	LDR  R10, [R1, R7, LSL #2]  @ get the digit of the guess sequence at the current index [1]
	                            @ (the address offset is index * 4; LSL #2 multiplies a value by 4)

	CMP  R8, R10                @ compare secret and guess sequence digits
	BNE  next_digit             @ if the digits are not the same, go to next digit
                                
								@ else, it is an exact match
	ADD  R4, #1                 @ add 1 to exact matches counter

	MOV  R2, R6                 @ set value of arguments to call 'matched' subroutine
	MOV  R3, R7                 @ (pointers to sequences are already set)
	PUSH {LR}
	BL   matched                @ mark current digits of both sequences as matched by setting them to zero
	POP  {LR}

next_digit:
	ADD  R6, #1                 @ update array indices
	ADD  R7, #1

	CMP  R6, #LEN               
	BEQ  reset_index            @ if all digits have been checked for exact matches, reset array indices for approx. matches
	B    exact                  @ else, loop around to compare the next digit

reset_index:
	CMP  R4, #LEN
	BEQ  match_done             @ if all digits were matched exactly, no need to check for approx. matches
	
	MOV  R6, #0                 @ reset array indices
	MOV  R7, #0

approx:
	LDR  R8, [R0, R6, LSL #2]   @ get the digit of the secret sequence at the current index [1]
	CMP  R8, #MATCHED
	BEQ  next_sec_d             @ if this digit has been matched (= #MATCHED), go to next secret sequence digit

compare:
	LDR  R10, [R1, R7, LSL #2]  @ get the digit of the guess sequence at the current index [1]
	CMP  R10, #MATCHED
	BEQ  next_guess_d           @ if this digit has been matched (= #MATCHED), go to next guess sequence digit

	CMP  R8, R10                @ compare secret and guess sequence digits
	BNE  next_guess_d           @ if the digits are not the same, go to next guess sequence digit

	                            @ else, it is an approximate match
	ADD  R5, #1                 @ add 1 to approximate matches counter
	
	MOV  R2, R6                 @ set value of arguments to call 'matched' subroutine
	MOV  R3, R7                 @ (pointers to sequences are already set)
	PUSH {LR}
	BL   matched                @ mark current digits (in R0 and R1) as matched by setting them to zero
	POP  {LR}
	B    next_sec_d             @ go to next secret sequence digit since current secret sequence digit has been matched

next_guess_d:
	ADD  R7, #1                 @ update guess sequence index
	CMP  R7, #LEN
	BEQ  next_sec_d             @ if all digits of guess sequence have been compared, go to next secret sequence digit
	B    compare                @ else, loop around to compare the next guess sequence digit

next_sec_d:
	ADD  R6, #1                 @ update secret sequence index
	CMP  R6, #LEN
	BEQ  match_done             @ if all digits of secret sequence have been compared, matching is done

	MOV  R7, #0                 @ reset guess sequence index
	B    approx                 @ loop around to compare the next secret sequence digit

match_done:
	MOV  R0, R4                 @ set output value for exact matches
	MOV  R1, R5                 @ set output value for approx. matches
	POP  {R4-R8, R10}           @ restore contents of registers used for this subroutine
	BX   LR                     @ return to main flow


matched:
	@ Marks a digit in the secret and guess sequence as matched (sets their value to #MATCHED)
	@ Register usage:
	@ R0  - pointer to secret sequence (argument)
	@ R1  - pointer to guess sequence (argument)
	@ R2  - array index for secret sequence element to mark (argument)
	@ R3  - array index for guess sequence element to mark (argument)
	@ R4  - value to set the marked digit as (= #MATCHED)

	PUSH {R4}                   @ preserve contents of registers used
	MOV  R4, #MATCHED
	STR  R4, [R0, R2, LSL #2]   @ mark the current digit of secret and guess sequence as matched (set them to #MATCHED)
	STR  R4, [R1, R3, LSL #2]
	POP  {R4}                   @ restore contents of registers used
	BX   LR                     @ return to line after subroutine call


display:
	@ Displays the number of exact and approximate matches on the HEX displays
	@ Register usage:
	@ R0  - number of exact matches (argument)
	@ R1  - number of approximate matches (argument)
	@ R4  - address of the hexbase
	@ R5  - address of the digit array
	@ R6  - hex display encoding for the number of exact matches
	@ R7  - hex display encoding for the number of approx matches


	@ TODO Display exact and approximate matches
	PUSH  {R4-R7}

	LDR R4, =HEXBASE			@ physical address of the hex
	LDR R5, =digits				@ physical address of the digits array		
	
	LDRB R6,[R5,R0]				@ Loads one byte from the address stored in R5, with an offset of R0
	LSL R6,#8					@ Shift 8 bytes to the left	

	LDRB R7,[R5,R1]				@ Loads one byte from the address stored in R5, with an offset of R1		
	ORR R6,R7					@ Logical needed to display the values in the right postion

	STR R6,[R4]					
	
	POP  {R4-R7}
	BX   LR						@ return to line after subroutine call

exit:	@MOV	 R0, R4		@ load result to output register
	MOV 	 R7, #1		@ load system call code
	SWI 	 0		@ return this value
	
@ =============================================================================

.data

@ constants about the basic setup of the game: length of sequence and number of colors	
.equ LEN, 3
.equ COL, 3
.equ NAN1, 8
.equ NAN2, 9

@ constants needed to interface with external devices	
.equ BUTTONBASE, 0xFF200050
.equ HEXBASE,	 0xFF200020
.equ BUTTON_NO,  1	

@ constant for marking a digit of the secret or guess sequence as matched
.equ MATCHED, -1

@ you probably want to define a table here, encoding the display of digits on the HEX display	
.align 1	
digits:
	.byte  0b0111111	@ 0
@	... COMPLETE THIS TABLE  ...
	.byte  0b0000110    @ 1
	.byte  0b1011011	@ 2
	.byte  0b1001111	@ 3
	.byte  0b1100110	@ 4
	.byte  0b1101101	@ 5
	.byte  0b1111101 	@ 6
	.byte  0b0000111	@ 7
	.byte  0b1111111	@ 8
	.byte  0b1100111	@ 9

@ INPUT DATA for the matching function
.align 4
secret: .word 1 
	.word 2 
	.word 1 

.align 4
guess:	.word 3 
	.word 1 
	.word 3 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 0 1
.align 4
expect: .byte 0
	.byte 1

.align 4
secret1: .word 1 
	.word 2 
	.word 3 

.align 4
guess1:	.word 1 
	.word 1 
	.word 2 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 1
.align 4
expect1: .byte 1
	.byte 1

.align 4
secret2: .word 2 
	.word 3
	.word 2 

.align 4
guess2:	.word 3 
	.word 3 
	.word 1 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 0
.align 4
expect2: .byte 1
	.byte 0

