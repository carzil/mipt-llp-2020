#pragma once

struct iframe {
    int ip;
    int cs;
    int flags;
    int sp;
    int ss;
};

void init_idt();
