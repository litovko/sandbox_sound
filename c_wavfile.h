#ifndef C_WAVFILE_H
#define C_WAVFILE_H

#include <QObject>
#include <QString>
#include <SDL.h>
#include "fasttransforms.h"
#include "qmlplot.h"
#include "qcustomplot.h"


class c_wavfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal frame_start READ frame_start WRITE setFrame_start NOTIFY frame_startChanged)
    Q_PROPERTY(qreal frame_width READ frame_width WRITE setFrame_width NOTIFY frame_widthChanged)
    Q_PROPERTY(int sample_rate READ sample_rate NOTIFY sample_rateChanged)
    Q_PROPERTY(CustomPlotItem *chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(CustomPlotItem *chartf READ chartf WRITE setChartf NOTIFY chartfChanged)
public:
    explicit c_wavfile(QObject *parent = nullptr);




    qreal frame_width() const;
    void setFrame_width(const qreal &frame_width);

    qreal frame_start() const;
    void setFrame_start(const qreal &frame_start);

    int sample_rate() const;

    CustomPlotItem *chart() const;
    void setChart(CustomPlotItem *chart);

    CustomPlotItem *chartf() const;
    void setChartf(CustomPlotItem *chartf);

signals:
    void sourceChanged();
    void resultChanged();
    void frame_startChanged();
    void frame_widthChanged();
    void sample_rateChanged();
    void chartChanged();
    void chartfChanged();

public slots:
    void load_file(const QString& filename);
    Q_INVOKABLE void fft();
    void draw_frame();
    void calc_feature1();
private:
    QVector<QPointF> m_points;
    QVector<QPointF> m_points2;
    QVector<QPointF> m_res_points;
    qreal m_xmax;
    qreal m_frame_start=1.2; //начало фрейма для анализа в секундах
    qreal m_frame_width=0.01; //ширина фрейма в секундах
    int m_sample_rate;
    Uint8 *wav_buffer;
    CustomPlotItem *m_chart=nullptr;
    CustomPlotItem *m_chartf=nullptr;
    QCPItemRect *rect = nullptr;
};

#endif // C_WAVFILE_H
