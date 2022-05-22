
#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"

VM vm;

static void resetStack() {
	vm.stackTop = vm.stack;
}

void push(Value value) {
	if (vm.stackTop < vm.stack + STACK_MAX) {
		*vm.stackTop++ = value;
	}
}

Value pop() {
	if (vm.stackTop > vm.stack) {
		return *(--vm.stackTop);
	}
}

void initVM() {
	resetStack();
}

void freeVM() {
}

static InterpretResult run() {

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
	do { \
		double b = pop(); \
		double a = pop(); \
		push(a op b); \
	} while(false)

	for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
		printf("        ");
		for (Value *stack = vm.stack; stack < vm.stackTop; stack++) {
			printf("[");
			printValue(*stack);
			printf("]");
		}
		printf("\n");
		disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
		uint8_t instruction;
		switch (instruction = READ_BYTE()) {
			case OP_CONSTANT: {
				Value constant = READ_CONSTANT();
				push(constant);
				break;
			}
			case OP_ADD: BINARY_OP(+); break;
			case OP_SUBTRACT: BINARY_OP(-); break;
			case OP_MULTIPLY: BINARY_OP(*); break;
			case OP_DIVIDE: BINARY_OP(/); break;
			case OP_NEGATE:
				push(-pop());
				break;
			case OP_RETURN:
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
		}
	}
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(const char *source) {
	compile(source);
	return INTERPRET_OK;
}

