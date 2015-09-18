/****************************************************************************
**
** Copyright (C) 2015 Andrey Kartashov .
** All rights reserved.
** Contact: Andrey Kartashov (porter@porter.st)
**
** This file is part of the averagedensity module of the genome-tools.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Andrey Kartashov.
**
****************************************************************************/
#ifndef _SubstringsH_
#define _SubstringsH_


#include <config.hpp>

#define FSTM Substrings


class vertex;
class vertex {

    public:
        int number;
        int lvl;
        QMap<QChar,QSharedPointer<vertex> > edge;
        vertex();
        vertex(int lvl, int number=1);
};

struct _StringWeight {
    QString string;
    int	weight;
};
typedef _StringWeight StringWeight;

class Mech {
    private:
        QVector < QSharedPointer<vertex> > ends;
	QList<StringWeight> _reps;
	
        int deep;

        void createDagU(QChar);
	void _getRepeats(QMap<QChar,QSharedPointer<vertex> >,QString,int);
    public:

        Mech(void);
        void createDag(QString, int);
	QList<StringWeight> getRepeats();
};


class FSTM: public QObject
{
 Q_OBJECT
 private:

  Mech mech;
  QFile infile;

 public:

public slots:
  void start(void);

signals:
  void finished(void);

public:
  FSTM(QObject* parent=0);
  ~FSTM();
};

#endif
