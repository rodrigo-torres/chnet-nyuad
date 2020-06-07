/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WIDGET_H
#define WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton_4;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QSpinBox *spinBox;
    QLabel *label_4;
    QSpinBox *spinBox_2;
    QWidget *page_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_2;
    QCheckBox *checkBox;
    QLabel *label_2;
    QCheckBox *checkBox_2;
    QSpinBox *spinBox_3;
    QLabel *label_3;
    QProgressBar *progressBar;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(789, 1048);
        verticalLayoutWidget = new QWidget(Form);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 10, 402, 733));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(verticalLayoutWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setMinimumSize(QSize(400, 400));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 396, 396));
        scrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_2->addWidget(scrollArea);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        toolButton_4 = new QToolButton(verticalLayoutWidget);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));

        horizontalLayout_3->addWidget(toolButton_4);

        toolButton_2 = new QToolButton(verticalLayoutWidget);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        horizontalLayout_3->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(verticalLayoutWidget);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_3->addWidget(toolButton_3);

        toolButton = new QToolButton(verticalLayoutWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        horizontalLayout_3->addWidget(toolButton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(verticalLayoutWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        gridLayout_3 = new QGridLayout(page);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_5 = new QLabel(page);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        spinBox = new QSpinBox(page);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        gridLayout_3->addWidget(spinBox, 0, 1, 1, 1);

        label_4 = new QLabel(page);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        spinBox_2 = new QSpinBox(page);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));

        gridLayout_3->addWidget(spinBox_2, 1, 1, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        gridLayout_2 = new QGridLayout(page_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(page_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        comboBox = new QComboBox(page_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout_2->addWidget(comboBox, 0, 1, 1, 1);

        label_6 = new QLabel(page_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 1, 0, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(page_2);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));

        gridLayout_2->addWidget(doubleSpinBox, 1, 1, 1, 1);

        label_7 = new QLabel(page_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 2, 0, 1, 1);

        doubleSpinBox_2 = new QDoubleSpinBox(page_2);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));

        gridLayout_2->addWidget(doubleSpinBox_2, 2, 1, 1, 1);

        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 3, 0, 1, 2);

        checkBox = new QCheckBox(verticalLayoutWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 2, 0, 1, 1);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 4, 0, 1, 1);

        checkBox_2 = new QCheckBox(verticalLayoutWidget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 2, 1, 1, 1);

        spinBox_3 = new QSpinBox(verticalLayoutWidget);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));

        gridLayout->addWidget(spinBox_3, 4, 1, 1, 1);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        progressBar = new QProgressBar(verticalLayoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout_2->addWidget(progressBar);


        retranslateUi(Form);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        toolButton_4->setText(QApplication::translate("Form", "...", nullptr));
        toolButton_2->setText(QApplication::translate("Form", "...", nullptr));
        toolButton_3->setText(QApplication::translate("Form", "...", nullptr));
        toolButton->setText(QApplication::translate("Form", "...", nullptr));
        label_5->setText(QApplication::translate("Form", "Higher integral limit:", nullptr));
        label_4->setText(QApplication::translate("Form", "Lower integral limit:", nullptr));
        label->setText(QApplication::translate("Form", "Import calibration parameters from:", nullptr));
        comboBox->setItemText(0, QApplication::translate("Form", "File header", nullptr));
        comboBox->setItemText(1, QApplication::translate("Form", "XRF spectra window", nullptr));

        label_6->setText(QApplication::translate("Form", "Lower limit:", nullptr));
        label_7->setText(QApplication::translate("Form", "Higher limit:", nullptr));
        checkBox->setText(QApplication::translate("Form", "Instrumental Bins", nullptr));
        label_2->setText(QApplication::translate("Form", "Pixel Size:", nullptr));
        checkBox_2->setText(QApplication::translate("Form", "Energy", nullptr));
        label_3->setText(QApplication::translate("Form", "Integral Filter Units", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WIDGET_H
