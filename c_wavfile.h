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
    Q_PROPERTY(QtCharts::QLineSeries *result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(qreal frame_start READ frame_start WRITE setFrame_start NOTIFY frame_startChanged)
    Q_PROPERTY(qreal frame_width READ frame_width WRITE setFrame_width NOTIFY frame_widthChanged)
    Q_PROPERTY(int sample_rate READ sample_rate NOTIFY sample_rateChanged)
public:
    explicit c_wavfile(QObject *parent = nullptr);
    QLineSeries *source() const;
    void setSource(QLineSeries *source);

    QLineSeries *result() const;
    void setResult(QLineSeries *result);

    qreal frame_width() const;
    void setFrame_width(const qreal &frame_width);

    qreal frame_start() const;
    void setFrame_start(const qreal &frame_start);

    int sample_rate() const;

signals:
    void sourceChanged();
    void resultChanged();
    void frame_startChanged();
    void frame_widthChanged();
    void sample_rateChanged();

public slots:
    void load_file(const QString& filename);
    Q_INVOKABLE void fft();
private:
    QLineSeries* m_source;
    QLineSeries* m_result;

    QVector<QPointF> m_points;
    QVector<QPointF> m_points2;
    QVector<QPointF> m_res_points;
    qreal m_xmax;
    qreal m_frame_start=0; //начало фрейма для анализа в секундах
    qreal m_frame_width=1; //ширина фрейма в секундах
    int m_sample_rate;
    Uint8 *wav_buffer;

};

#endif // C_WAVFILE_H
