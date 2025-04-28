#include "mainwidget.h"
#include "parse.h"
#include "expr.h"
#include "val.h"
#include "env.h"
#include <sstream>
#include <stdexcept>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    setupLayout();

    connect(submitButton, &QPushButton::clicked, this, &MainWidget::handleSubmit);
    connect(resetButton, &QPushButton::clicked, this, &MainWidget::handleReset);
}

void MainWidget::setupLayout() {
    QLabel *titleLabel = new QLabel("MSDscript by Thomas Ho: A Program Interface");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 18px;");

    QLabel *expressionLabel = new QLabel("Enter Expression:");
    expressionLabel->setStyleSheet("font-weight: bold;");
    expressionInput = new QTextEdit;

    QLabel *resultsLabel = new QLabel("Results:");
    resultsLabel->setStyleSheet("font-weight: bold;");
    resultsOutput = new QTextEdit;
    resultsOutput->setReadOnly(true);

    QLabel *chooseLabel = new QLabel("Choose:");
    chooseLabel->setStyleSheet("font-weight: bold;");
    interpRadio = new QRadioButton("Interp");
    prettyPrintRadio = new QRadioButton("Pretty Print");
    interpRadio->setChecked(true);

    QVBoxLayout *radioLayout = new QVBoxLayout;
    radioLayout->addWidget(interpRadio);
    radioLayout->addWidget(prettyPrintRadio);

    QHBoxLayout *chooseLayout = new QHBoxLayout;
    chooseLayout->addWidget(chooseLabel);
    chooseLayout->addLayout(radioLayout);

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    chooseLayout->addSpacerItem(spacer);

    submitButton = new QPushButton("Submit");
    resetButton = new QPushButton("Reset");

    submitButton->setFixedSize(70, 30);
    resetButton->setFixedSize(70, 30);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(titleLabel);
    layout->addWidget(expressionLabel);
    layout->addWidget(expressionInput);
    layout->addLayout(chooseLayout);
    layout->addWidget(submitButton);
    layout->addWidget(resultsLabel);
    layout->addWidget(resultsOutput);
    layout->addWidget(resetButton);

    setLayout(layout);
}


void MainWidget::handleSubmit() {
    QString input = expressionInput->toPlainText();
    std::string result;

    try {
        PTR(Expr) e = parse_str(input.toStdString());

        if (interpRadio->isChecked()) {
            result = e->interp(Env::empty)->to_string();
        } else {
            // Pretty-print expression
            std::stringstream ss;
            std::streampos dummyIndent = 0;
            e->pretty_print(ss, prec_none, dummyIndent);
            result = ss.str();
        }

        resultsOutput->setText(QString::fromStdString(result));
    } catch (std::runtime_error &err) {
        QMessageBox::critical(this, "Error", err.what());
    }
}

void MainWidget::handleReset() {
    expressionInput->clear();
    resultsOutput->clear();
    interpRadio->setChecked(true);
}
