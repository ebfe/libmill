/*

  Copyright (c) 2015 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#ifndef MILL_H_INCLUDED
#define MILL_H_INCLUDED 

/******************************************************************************/
/*  Coroutines                                                                */
/******************************************************************************/

void *mill_go_prologue(void);
void mill_go_epilogue(void);

#define go(fn) \
    do {\
        void *mill_sp = mill_go_prologue();\
        if(mill_sp) {\
            volatile int mill_unoptimisable = 1;\
            int mill_anchor[mill_unoptimisable];\
            char mill_filler[(char*)&mill_anchor - (char*)(mill_sp)];\
            fn;\
            mill_go_epilogue();\
        }\
    } while(0)

void yield(void);

/******************************************************************************/
/*  Channels                                                                  */
/******************************************************************************/

typedef struct chan *chan;

chan chmake(void);
void chaddref(chan ch);
void chs(chan ch, void *val);
void *chr(chan ch);
void chclose(chan ch);

#define mill_concat(x,y) x##y

#define chselect \
    for(chpoll_start();;) {\
        if(chpoll_done()) {\
            if(0)

#define mill_chselect_item(type, chan, seqnum) \
                chpoll_end();\
                break;\
            }\
            goto mill_concat(label, seqnum);\
        }\
        chpoll_##type(chan, seqnum);\
        if(0) {\
            mill_concat(label, seqnum):\
            if(chpoll_result() == seqnum) {\
                mill_concat(dummylabel, seqnum)

#define in(chan) mill_chselect_item(in, (chan), __COUNTER__)
#define out(chan) mill_chselect_item(out, (chan), __COUNTER__)

#define end \
                chpoll_end();\
                break;\
            }\
            assert(0);\
        }\
        chpoll_select();

void chpoll_start(void);
int chpoll_done(void);
int chpoll_result(void);
void chpoll_in(chan ch, int idx);
void chpoll_out(chan ch, int idx);
void chpoll_select(void);
void chpoll_end(void);

#endif

