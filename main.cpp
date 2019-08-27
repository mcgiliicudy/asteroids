/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "window.h"
#include "mainwindow.h"

#include "mos6502.h"
#include "mos6502disassembler.h"

#include "inputkeys.h"

#include <QApplication>
#include <QSurfaceFormat>

Window *window;

Keys pressedKeys;

uint8_t mem[1024*64];


void busWrite(uint16_t addr, uint8_t val)
{
    static int thump = 0;

    if (addr>=0x2000 && addr <0x4000)
    {
        switch(addr)
        {
            case 0x2007:
                0;
                break;
            case 0x3000:
                window->updateDVG(mem);
                break;

            case 0x3C01:
                if ((val>0) & !(window->sndSaucerBig.isPlaying()))
                    window->sndSaucerBig.play();
                break;

            case 0x3C03:
                if ((val>0) & !(window->sndThrust.isPlaying()))
                    window->sndThrust.play();
                break;

            case 0x3C04:
                if ((val>0) & !(window->sndFire.isPlaying()))
                    window->sndFire.play();
                break;

            case 0x3A00:
                thump++;
                if ((thump % 2)==0)
                {
                    if ((val>0) & !(window->sndBeat1.isPlaying()))
                        window->sndBeat1.play();
                }
                else
                {
                    if ((val>0) & !(window->sndBeat2.isPlaying()))
                        window->sndBeat2.play();
                }
                break;

            case 0x3C00:
                if ((val>0) & !(window->sndSaucerSmall.isPlaying()))
                    window->sndSaucerSmall.play();
                break;

            case 0x3600:
                if ((val>0 & val<128) & !(window->sndBangBig.isPlaying()))
                    window->sndBangBig.play();
                break;

            case 0x3C05:
                if ((val>0) & !(window->sndExtraShip.isPlaying()))
                    window->sndExtraShip.play();
                break;



        }

    }
    else
    {
        mem[addr] = val;
    }


};

uint8_t busRead(uint16_t addr)
{
    if (addr>=0x2000 && addr <0x4000)
    {
        switch(addr)
        {
            case 0x2007:
                return 0;
            case 0x3000:
                return 0;
            case 0x2406:
                if (pressedKeys.Right) return 255;
                else                   return 0;
            case 0x2407:
                if (pressedKeys.Left)  return 255;
                else                   return 0;
            case 0x2403:
                if (pressedKeys.Player1)  return 255;
                else                      return 0;
            case 0x2004:
                if (pressedKeys.Fire)  return 255;
                else                   return 0;
            case 0x2405:
                if (pressedKeys.Thrust) return 255;
                else                    return 0;


        }

    }
    else
    {
        return mem[addr];
    }

    return 0;
};

void readRom(void)
{
    for (int i=0; i<64*1024; i++) mem[i] = 0;

    const char *romFileNames[] = {"035127.02", "035143.02", "035144.02", "035145.02"};
    for (int i=0; i<4; i++)
    {
        char fileName[1024];
        sprintf(fileName, "/media/jmg/New450/src/tmp/qtcreatortest/2dpainting/%s", romFileNames[i]);
        FILE *file = fopen(fileName, "r");
        for (int j=0; j<2048; j++)
        {
            uint8_t c;
            fread(&c, 1, 1, file);

            if      (i==0) mem[0x800  + j] = c;
            else if (i==1) mem[0x7800 + j] = c;
            else if (i==2) mem[0x7000 + j] = c;
            else           mem[0x6800 + j] = c;

            if      (i==0) mem[0x5000  + j] = c;


        }
        fclose(file);
    }


}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    mos6502 cpu(busRead, busWrite);

    uint16_t pc = 0x6800;

    readRom();

    DVG dvg;


    MainWindow MainWindow;
    MainWindow.show();

    window = new Window(dvg);
    window->show();

    window->updateDVG(mem);


    cpu.Run(1000000000, &MainWindow);



    char output[10000];

    options_t options;

    options.org=0x6800;
    options.offset=0;
    options.hex_output=true;
    options.cycle_counting=true;

    disassemble(output, mem, &options, &pc);
    pc++;
    disassemble(output, mem, &options, &pc);
    pc++;
    disassemble(output, mem, &options, &pc);
    pc++;
    disassemble(output, mem, &options, &pc);
    pc++;
    disassemble(output, mem, &options, &pc);
    pc++;
    disassemble(output, mem, &options, &pc);
    pc++;




    return app.exec();
}
