#ifndef C_WAVFILE_H
#define C_WAVFILE_H

#include <QObject>
#include <QString>
#include <SDL.h>
#include <QtCharts>
#include <QtCharts/QChartView>

#include "fasttransforms.h"

using namespace QtCharts;

class c_wavfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QtCharts::QLineSeries *source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit c_wavfile(QObject *parent = nullptr);
    QLineSeries *source() const;
    void setSource(QLineSeries *source);

signals:
    void sourceChanged();

public slots:
    void load_file(const QString& filename);
private:
    QLineSeries* m_source;

    QVector<QPointF> m_points;
    qreal m_xmax;

};

#endif // C_WAVFILE_H
