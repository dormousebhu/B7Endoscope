#include "FocusToolBar.h"
#include <QDebug>

FocusToolBar::FocusToolBar(QWidget *parent)
    :QToolBar(parent)
{
    for(int i = 0; i < 6; i++)
    {
        m_pAction[i] = new QAction(tr("Focus"), this);
        m_pAction[i]->setToolTip(tr("Focus CH%1 camera.").arg(i+1));
        addAction(m_pAction[i]);
        connect(m_pAction[i], &QAction::triggered, this, &FocusToolBar::onAction);
    }
    m_pAction[0]->setIcon(QIcon(":/res/set256-1.png"));
    m_pAction[1]->setIcon(QIcon(":/res/set256-2.png"));
    m_pAction[2]->setIcon(QIcon(":/res/set256-3.png"));
    m_pAction[3]->setIcon(QIcon(":/res/set256-4.png"));
    m_pAction[4]->setIcon(QIcon(":/res/set256-5.png"));
    m_pAction[5]->setIcon(QIcon(":/res/set256-6.png"));
}

void FocusToolBar::onAction(bool checked )
{
    qDebug() << "FocusToolBar::onAction";
    Q_UNUSED(checked);
    for(int i = 0; i < 6; i++)
    {
        if(sender() == m_pAction[i])
        {
            emit showPage(i + 1);
            return;
        }
    }
}
