/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *pwd_le;
    QLineEdit *name_le;
    QLabel *pwd_lab;
    QLabel *name_lab;
    QPushButton *login_pb;
    QHBoxLayout *horizontalLayout;
    QPushButton *regist_pb;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_pb;

    void setupUi(QMainWindow *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName("TcpClient");
        TcpClient->resize(353, 208);
        centralwidget = new QWidget(TcpClient);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        pwd_le = new QLineEdit(centralwidget);
        pwd_le->setObjectName("pwd_le");
        QFont font;
        font.setPointSize(14);
        font.setBold(false);
        font.setItalic(false);
        pwd_le->setFont(font);
        pwd_le->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(pwd_le, 1, 1, 1, 1);

        name_le = new QLineEdit(centralwidget);
        name_le->setObjectName("name_le");
        name_le->setEnabled(true);
        name_le->setMinimumSize(QSize(200, 30));
        name_le->setFont(font);

        gridLayout->addWidget(name_le, 0, 1, 1, 1);

        pwd_lab = new QLabel(centralwidget);
        pwd_lab->setObjectName("pwd_lab");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pwd_lab->sizePolicy().hasHeightForWidth());
        pwd_lab->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(false);
        font1.setItalic(false);
        pwd_lab->setFont(font1);

        gridLayout->addWidget(pwd_lab, 1, 0, 1, 1);

        name_lab = new QLabel(centralwidget);
        name_lab->setObjectName("name_lab");
        name_lab->setEnabled(true);
        sizePolicy.setHeightForWidth(name_lab->sizePolicy().hasHeightForWidth());
        name_lab->setSizePolicy(sizePolicy);
        name_lab->setFont(font1);

        gridLayout->addWidget(name_lab, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        login_pb = new QPushButton(centralwidget);
        login_pb->setObjectName("login_pb");
        QFont font2;
        font2.setPointSize(20);
        font2.setBold(false);
        font2.setItalic(false);
        login_pb->setFont(font2);

        verticalLayout->addWidget(login_pb);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        regist_pb = new QPushButton(centralwidget);
        regist_pb->setObjectName("regist_pb");
        regist_pb->setFont(font);

        horizontalLayout->addWidget(regist_pb);

        horizontalSpacer = new QSpacerItem(68, 66, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancel_pb = new QPushButton(centralwidget);
        cancel_pb->setObjectName("cancel_pb");
        cancel_pb->setFont(font);

        horizontalLayout->addWidget(cancel_pb);


        verticalLayout->addLayout(horizontalLayout);

        TcpClient->setCentralWidget(centralwidget);

        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QMainWindow *TcpClient)
    {
        TcpClient->setWindowTitle(QCoreApplication::translate("TcpClient", "TcpClient", nullptr));
        pwd_lab->setText(QCoreApplication::translate("TcpClient", "\345\257\206   \347\240\201\357\274\232", nullptr));
        name_lab->setText(QCoreApplication::translate("TcpClient", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        login_pb->setText(QCoreApplication::translate("TcpClient", "\347\231\273    \345\275\225", nullptr));
        regist_pb->setText(QCoreApplication::translate("TcpClient", "\346\263\250\345\206\214", nullptr));
        cancel_pb->setText(QCoreApplication::translate("TcpClient", "\346\263\250\351\224\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
