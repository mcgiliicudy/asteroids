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

#include "glwidget.h"
#include "widget.h"
#include "window.h"
#include "qevent.h"
#include "inputkeys.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>

#include <QCoreApplication>


Widget *native;

extern Keys pressedKeys;

//! [0]
Window::Window(DVG &advg): dvg(advg)
{
    setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

    native = new Widget(&helper, this, dvg, galaxianShip);
//    GLWidget *openGL = new GLWidget(&helper, this);
    QLabel *nativeLabel = new QLabel(tr("Native"));
    nativeLabel->setAlignment(Qt::AlignHCenter);
//    QLabel *openGLLabel = new QLabel(tr("OpenGL"));
//    openGLLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(native, 0, 0);
//    layout->addWidget(openGL, 0, 1);
    layout->addWidget(nativeLabel, 1, 0);
//    layout->addWidget(openGLLabel, 1, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, native, &Widget::animate);
    //connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    timer->start(50);


    sndFire       .setSource(QUrl::fromLocalFile("fire.wav"));
    sndThrust     .setSource(QUrl::fromLocalFile("thrust.wav"));
    sndBeat1      .setSource(QUrl::fromLocalFile("beat1.wav"));
    sndBeat2      .setSource(QUrl::fromLocalFile("beat2.wav"));
    sndSaucerSmall.setSource(QUrl::fromLocalFile("saucerSmall.wav"));
    sndSaucerBig  .setSource(QUrl::fromLocalFile("saucerBig.wav"));
    sndBangSmall  .setSource(QUrl::fromLocalFile("bangSmall.wav"));
    sndBangMedium .setSource(QUrl::fromLocalFile("bangMedium.wav"));
    sndBangBig    .setSource(QUrl::fromLocalFile("bangLarge.wav"));
    sndExtraShip  .setSource(QUrl::fromLocalFile("extraShip.wav"));
    sndFire       .setVolume(0.5);
    sndThrust     .setVolume(0.5);
    sndBeat1      .setVolume(0.5);
    sndBeat2      .setVolume(0.5);
    sndSaucerSmall.setVolume(0.5);
    sndSaucerBig  .setVolume(0.5);
    sndBangSmall  .setVolume(0.5);
    sndBangMedium .setVolume(0.5);
    sndBangBig    .setVolume(0.5);
    sndExtraShip  .setVolume(0.5);

    while (!sndFire       .isLoaded()) QCoreApplication::processEvents();
    while (!sndThrust     .isLoaded()) QCoreApplication::processEvents();
    while (!sndBeat1      .isLoaded()) QCoreApplication::processEvents();
    while (!sndBeat2      .isLoaded()) QCoreApplication::processEvents();
    while (!sndSaucerSmall.isLoaded()) QCoreApplication::processEvents();
    while (!sndSaucerBig  .isLoaded()) QCoreApplication::processEvents();
    while (!sndBangSmall  .isLoaded()) QCoreApplication::processEvents();
    while (!sndBangMedium .isLoaded()) QCoreApplication::processEvents();
    while (!sndBangBig    .isLoaded()) QCoreApplication::processEvents();
    while (!sndExtraShip  .isLoaded()) QCoreApplication::processEvents();

    galaxianShip.load("galaxians.png");



}

void Window::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_1:
            pressedKeys.Player1 = true;
            break;

        case Qt::Key_A:
            pressedKeys.Left = true;
            break;

        case Qt::Key_S:
            pressedKeys.Right = true;
            break;

        case Qt::Key_Up:
            pressedKeys.Fire = true;
            break;

        case Qt::Key_Down:
            pressedKeys.Thrust = true;
            break;
    }



}


void Window::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_1:
        pressedKeys.Player1 = false;
        break;

    case Qt::Key_A:
        pressedKeys.Left = false;
        break;

    case Qt::Key_S:
        pressedKeys.Right = false;
        break;

    case Qt::Key_Up:
        pressedKeys.Fire = false;
        break;

    case Qt::Key_Down:
        pressedKeys.Thrust = false;
        break;
    }

}



void Window::updateDVG(uint8_t *amem)
{
    native->addMem(amem);
}
