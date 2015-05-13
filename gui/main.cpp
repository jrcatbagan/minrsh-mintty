//
// File: main.cpp
// Date: 2015, May 09
//
// Copyright (C) 2015, Irene Cho <irenechoster@gmail.com>
// Copyright (C) 2015, Edward Casilio <ed.rambozzo@gmail.com>
// Copyright (C) 2015, Jarielle Catbagan <jrcatbagan@ca.rr.com>

#include "mintty.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mintty w;
    w.show();

    return a.exec();
}
