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

#include "widget.h"
#include "helper.h"

#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QBitmap>
#include <QPixmap>

#include <stack>

#include <stdint.h>


#define CUR  0xA
#define HALT 0xB
#define JSR  0xC
#define RTS  0xD
#define JMP  0xE
#define SVEC 0xF



static uint16_t mem[64*1024];

std::stack<uint16_t> pcStack;

//! [0]
Widget::Widget(Helper *helper, QWidget *parent, DVG &advg, QPixmap &AgalaxianShip)
    : QWidget(parent), helper(helper), dvg(advg), galaxianShip(AgalaxianShip)
{
    QColor colorBackground(10, 2, 15);

    elapsed = 0;
    setFixedSize(1024, 1024);
    for (int i=0; i<1024; i++) mem[i] = 0;

    background = QBrush(colorBackground);
    circlePen = QPen(QColor(255,255,150,100));
    circlePen.setWidth(1);
    circlePen.setCapStyle(Qt::RoundCap);
    circlePen.setJoinStyle(Qt::RoundJoin);

    bitmap = new QPixmap(1024, 1024);
    bitmap->fill(colorBackground);
}
//! [0]

//! [1]
void Widget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 100000;
    update();
}
//! [1]

//! [2]
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    QPainter screen;
    screen.begin(this);
//    painter.begin(this);
    painter.begin(bitmap);
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.setRenderHint(QPainter::HighQualityAntialiasing);



    double x, y;
    int s, s0;
    double x0=0, y0=0;
    double scale = 1.0;


    painter.fillRect(event->rect(), QColor(0,0,0,150));
//    painter.translate(512, 512);

    int i = 0;
    uint16_t pc = 0x4000;
    while (i<1023)
    {
        i++;

        auto B = mem[pc];
        pc++;

        auto A = mem[pc];
        uint8_t a0 = A >> 4;
        uint8_t a1 = A & 0xF;
        pc++;

        if (a0<=9)
        {
            auto D = mem[pc];
            pc++;
            auto C = mem[pc];
            uint8_t c0 = C >> 4;
            uint8_t c1 = C & 0xF;
            pc++;

            int signy = ((a1 & 4)>0)?-1:1;
            int signx = ((c1 & 4)>0)?-1:1;

            s = a0;

            int intScale = ((s - 9) + s0) & 0xF;
            if (intScale<8)
            {
                scale = 1 << intScale;
            }
            else
            {
                scale = 1.0/(1 << (16-intScale));
            }


            y = signy*(((int(a1 & 3)) << 8) + B)*scale;
            x = signx*(((int(c1 & 3)) << 8) + D)*scale;

            y = y + y0;
            x = x + x0;




            circlePen.setWidthF(c0/1.4+0.0001);
            circlePen.setColor(QColor(105,105,255,100));
            painter.setPen(circlePen);
            painter.drawLine(QPointF(x0, 1024.0 - y0), QPointF(x+0.1, 1024.0 - y));


            circlePen.setWidthF(c0/2.5+0.0001);
            circlePen.setColor(QColor(205,205,255,100));
            painter.setPen(circlePen);
            painter.drawLine(QPointF(x0, 1024.0 - y0), QPointF(x+0.1, 1024.0 - y));


            circlePen.setWidthF(c0/4.0+0.0001);
            circlePen.setColor(QColor(235,235,255,255));
            painter.setPen(circlePen);
            painter.drawLine(QPointF(x0, 1024.0 - y0), QPointF(x+0.1, 1024.0 - y));

            x0 = x;
            y0 = y;

        }
        else if (a0==CUR)
        {
            auto D = mem[pc];
            pc++;
            auto C = mem[pc];
            uint8_t c0 = C >> 4;
            uint8_t c1 = C & 0xF;
            pc++;

            y0 = (uint16_t(a1 & 3) << 8) + B;
            x0 = (uint16_t(c1 & 3) << 8) + D;
            s0 = c0;

        }
        else if (a0==HALT)
        {
            break;
        }
        else if (a0==JSR)
        {
            pcStack.push(pc);
            pc = (uint16_t(a1 << 8) + B)*2 + 0x4000;

        }
        else if (a0==RTS)
        {
            pc = pcStack.top();
            pcStack.pop();
        }
        else if (a0==JMP)
        {
               pc = (uint16_t(a1 << 8) + B)*2 + 0x4000;
        }
        else if (a0==SVEC)
        {
            uint8_t b0 = B >> 4;
            uint8_t b1 = B & 0xF;

            int signy = ((a1 & 4)>0)?-1:1;
            int signx = ((b1 & 4)>0)?-1:1;

            s = ((b1 & 8) >> 2) + ((a1 & 8) >> 3);

            int intScale = ((s + 9) + s0) & 0xF;
            if (intScale<8)
            {
                scale = 1 << intScale;
            }
            else
            {
                scale = 1.0/(1 << (16-intScale));
            }

            y = 256*signy*(a1 & 3)*scale;
            x = 256*signx*(b1 & 3)*scale;

            y += y0;
            x += x0;

            circlePen.setWidthF(b0/1.4+0.0001);
            circlePen.setColor(QColor(105,105,255,100));
            painter.setPen(circlePen);
            painter.drawLine(QPointF(x0, 1024.0 - y0), QPointF(x, 1024.0 - y));

            circlePen.setWidthF(b0/2.5+0.0001);
            circlePen.setColor(QColor(205,205,255,100));
            painter.setPen(circlePen);
            painter.drawLine(QPointF(x0, 1024.0 - y0), QPointF(x, 1024.0 - y));

            circlePen.setWidthF(b0/4.0+0.0001);
            circlePen.setColor(QColor(245,245,255,255));
            painter.setPen(circlePen);
            painter.drawLine(QPointF(x0, 1024.0 - y0), QPointF(x, 1024.0 - y));



            x0 = x;
            y0 = y;

        }


    }

    painter.translate((256*mem[0x0284] + mem[0x02CA])/8, -128 + 1024 - (256*mem[0x02A7] + mem[0x02ED])/8);
    painter.rotate(-360*mem[0x61]/256.0 + 90);

    painter.drawPixmap(QPointF(-16.0, -23.5), galaxianShip);


//    helper->paint(&painter, event, elapsed/10.0, dvg);
    painter.end();

    screen.drawImage(0,0,this->bitmap->toImage());

    screen.end();


//    bitmap->fill(this->bitmap, 0, 0);
}
//! [2]


void Widget::addMem(uint8_t *amem)
{
  for (int i=0; i<64*1024; i++) mem[i] = amem[i];

}
