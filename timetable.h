#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QWidget>
#include <QLayout>

class Timetable : public QWidget
{
  Q_OBJECT
public:
  explicit Timetable(QWidget *parent = nullptr);

  QVBoxLayout *layout;

signals:

public slots:
};

#endif // TIMETABLE_H
