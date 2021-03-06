#include "servergame.h"
#include "ui_game.h"
#include "card.h"

#include <QTime>
#include <QStringList>


ServerGame::ServerGame(QWidget *parent) :
    Game(parent)
{
    server = new Server();

    connect(ui->lineEdit,SIGNAL(on_lineEdit_returnPressed()),this,SLOT(on_lineEdit_returnPressed()));

    //signali sa servera
    connect(server,SIGNAL(newMessage(QString)),this,SLOT(appendMessage(QString)));
    connect(server,SIGNAL(cardThrown(QString)),this,SLOT(addCard(QString)));
    connect(server,SIGNAL(groupThrown(QString)),this,SLOT(addGroupOfCards(QString)));

    //signali iz game-a
    connect(this,SIGNAL(onCardThrown(QString)),this,SLOT(sendCard(QString)));
    connect(this,SIGNAL(onGroupOfCardsThrown(QString)),this,SLOT(sendGroupOfCards(QString)));


} // END CONSTRUCTOR

void ServerGame::appendMessage(const QString &message)
{
    ui->textEdit->append(message);
}

void ServerGame::on_lineEdit_returnPressed()
{
    ui->textEdit->append(ui->lineEdit->text());

    QString s(ui->lineEdit->text());
    server->sendMessage(s);
}

void ServerGame::sendCard(const QString& card)
{
    qDebug() << "Stigo do dela da posalje kartu -> " + card;
    server->sendCard(card);
}

void ServerGame::addCard(const QString &card)
{
    Card* c = createCardByString(card);

    talon->addCard(c,true);
}

void ServerGame::addGroupOfCards(const QString &cards)
{

    QStringList list = cards.split(' ');

    int w1 = list.size() * 20;
    int pos_x = std::accumulate(table.begin() + table.size() / 3 * 3,
                                table.end(),
                                200,
                                [](const int& a, CardTableContainer* cdc)
                                    { return a + cdc->getContainerWidth(); } );
    int pos_y = 150 + (table.size() / 3 ) * 100;

    CardTableContainer* cdc =
            new CardTableContainer(this, pos_x, pos_y, w1, 100);


    for(int i=0; i<list.size()-1; i++)
    {
        Card* c = createCardByString(list.at(i));
        cdc->addCard(c,true);
    }

    table.append(cdc);
}

void ServerGame::sendGroupOfCards(const QString& cards)
{
    server->sendGroupOfCards(cards);
}

