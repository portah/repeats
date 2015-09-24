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


#include "substrings.hpp"

FSTM::FSTM(QObject *parent):
    QObject(parent)
{
}

FSTM::~FSTM()
{
}


void FSTM::start()
{
    QFile inFile;
    if(gArgs().getArgs("in").toString().isEmpty()) {
	inFile.open(stdin, QIODevice::ReadOnly);
    } else {
	inFile.setFileName(gArgs().getArgs("in").toString());
	inFile.open(QIODevice::ReadOnly|QIODevice::Text);
    }
    if(!inFile.isOpen()) {
        qDebug()<<"Error open file";
        emit finished();
        return;
    }

    QString str;
    QString head;
    
    while (!inFile.atEnd() || (!str.isEmpty())) {
	QByteArray line;
	if(!inFile.atEnd() ){
    	    line = inFile.readLine();
    	    if(line.isEmpty()) continue;
        }
        if(line.startsWith('#')) continue;
        if(line.endsWith('\n'))
           line.chop(1);
        if(line.startsWith('>') || inFile.atEnd()) {
    	    if(str.isEmpty()) { 
    		head=line;
    	        continue;
    	    }
	    Mech mech;
	    qDebug()<<"Working on:"<<str;
	    qDebug()<<"DAG creation started";
	    QTime tm;
	    tm.start();
	    mech.createDag(str,gArgs().getArgs("max").toInt());
	    qDebug()<<"DAG creation spent time:"<<tm.elapsed()<<" ms";
    
	    QList<StringWeight> r=mech.getRepeats();
	    QTextStream(stdout) << head<< endl;
	    foreach(StringWeight sw,r) {
		QTextStream(stdout) << sw.string <<"\t"<<sw.weight<< endl;
	    }
	    qDebug()<<"DAG printing spent time:"<<tm.elapsed()<<" ms";
	    qDebug()<<"Found #:"<<r.size()<<" repeats";
    
    	    head=line; 
    	    str="";
    	    continue;
    	}
        str.append(line);
        }
    emit finished();
}
/*

*/
Mech::Mech()
{
    ends.append(QSharedPointer<vertex>(new vertex()));
    deep=0;
}

void Mech::createDagU(QChar _chr){
    QMutableVectorIterator<QSharedPointer<vertex> > e(ends);
    while(e.hasNext()){
        QSharedPointer<vertex> i=e.next();
        int lvl = i->lvl;

        if(lvl >= deep) {
            e.remove();
            continue;
        }

        if(lvl == 0)
            i->number++;

        if(i->edge.contains(_chr)) {
            i->edge[_chr]->number++;
        } else {
            i->edge[_chr]=QSharedPointer<vertex>(new vertex(lvl+1,1));
        }

        if(lvl!=0) {
            e.setValue(i->edge[_chr]);
        } else {
    	    e.insert(i->edge[_chr]);
        }
    }
}

void Mech::createDag(QString _seq,int _deep) {
    deep=_deep;
    if(_seq.isEmpty()) return;

    for(int i=0; i<_seq.length();i++)
        createDagU(_seq.at(i));
}

QList<StringWeight> Mech::getRepeats() {
    
    for(QMap<QChar,QSharedPointer<vertex> >::iterator i = ends.at(0)->edge.begin();i!=ends.at(0)->edge.end();i++) {
	if(i.value()->number>1){
    	    _getRepeats(i.value()->edge,i.key(),i.value()->number);
        }
    }
    return _reps;
}

void Mech::_getRepeats(QMap<QChar,QSharedPointer<vertex> > cur,QString ch,int num) {
    int min=gArgs().getArgs("min").toInt()+1;
    if(cur.size()==0) return;
    StringWeight sw;
    bool prn=true;

    for(QMap<QChar,QSharedPointer<vertex> >::iterator i = cur.begin();i!=cur.end();i++) {
        if(i.value()->edge.size()==0 && i.value()->lvl>=min && i.value()->number>1) {
    	    sw.string=ch+i.key();
    	    sw.weight=i.value()->number;
    	    _reps<<sw;
            continue;
        }

        if(i.value()->lvl>=min && i.value()->number<num && prn) {
	    sw.string=ch;
    	    sw.weight=num;
    	    _reps<<sw;
    	    prn=false;
        }
        
        if(i.value()->edge.size()==0) continue;
        
        if(i.value()->number>1) 
    	    _getRepeats(i.value()->edge,ch+i.key(),i.value()->number);
        
    }
}

vertex::vertex(int lvl, int number)
{
    this->lvl=lvl;
    this->number=number;
}

vertex::vertex()
{
    this->lvl=0;
    this->number=0;
}
