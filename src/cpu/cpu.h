//
// Created by yarrya on 27.01.2021.
//

#ifndef CPU_CPU_H
#define CPU_CPU_H


#include "stack.h"

#define HLT     0
#define IN      1
#define OUT     2
#define ADD     3
#define SUB     4
#define MUL     5
#define DIV     6
#define JMP     7
#define J       8
#define JN      9
#define PUSH    10
#define POP     11
#define MOV     12

class cpu : stack {
private:
    size_t A = 0, B = 0, C = 0, D = 0;
    bool Zero = true, Carry = false, Fault = false;

    void show() {}

public:
    void upload(char* filename) {
        FILE* f = nullptr;
        if ((f = fopen(argv[1], "r")) == nullptr)
            throw std::logic_error("cpu error: can't open file\n");
        while ((x = getc(f)) != EOF)
            push(x);
        fclose(f);
        D = size() - 1;
        C = 0;
    }

    void step() {
        if (C < 0 || C > current)
            throw std::logic_error("cpu error: can't execute command\n");
        switch (data[C]) {
            case HLT: return;
            case IN: break;
            case OUT: break;
            case ADD: break;
            case SUB: break;
            case MUL: break;
            case DIV: break;
            case JMP: break;
            case J: break;
            case JN: break;
            case PUSH: break;
            case POP: break;
            case MOV: break;
        }
#ifdef DEBUG
        show();
#endif
    }
};


#endif //CPU_CPU_H
