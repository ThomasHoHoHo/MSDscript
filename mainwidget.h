// mainwidget.h
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QString>

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

private slots:
    void handleSubmit();
    void handleReset();

private:
    void setupLayout();

    QTextEdit *expressionInput;
    QTextEdit *resultsOutput;
    QRadioButton *interpRadio;
    QRadioButton *prettyPrintRadio;
    QPushButton *submitButton;
    QPushButton *resetButton;
};

#endif // MAINWIDGET_H
