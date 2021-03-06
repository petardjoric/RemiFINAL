#include "group.h"

Group::Group()
    : cards(), value(0)
{}

bool Group::addCard(Card *card){

    for(int i=0; i<cards.size(); i++)
        if(cards[i] == card)
            return false;

    cards.push_back(card);
    return true;
}

bool Group::removeCard(Card* card)
{
    for(int i=0; i<cards.size(); i++)
        if(cards[i] == card) {
            if(card->getSign() == Card::JOKER) //OVDE JE PROMENJENO
                card->setValue(0);
            if(card->getValue() == 15)
                card->setValue(1);
            cards.erase(cards.begin() + i);
            break;
        }

    //???????????????????????????????????????????????????
    if(cards.size() == 0)
        return true;
    return false;
}

void Group::clearGroup() { cards.clear(); }

void Group::sortGroup()
{
    std::sort(cards.begin(), cards.end(),
        [](Card* a, Card* b){return a->getValue() < b->getValue();});
}

void Group::correctValues()
{
    for(int i=0; i<cards.size(); i++)
    {
        if(cards[i]->getSign() == Card::JOKER)
            cards[i]->setValue(0);
        if(cards[i]->getNumber() == 1)
            cards[i]->setValue(1);
    }
}


/*
 * Funkcija vraca :
 *  -1 ako grupa sadrzi manje od 3 karte
 *  -2 ako grupa sadrzi vise od 1 JOKER-a
 *  -3 niti su istog znaka niti su svi razlicitog
 *  -4 razlicitog znaka ali i razlicite vr
 *  -5 istog znaka ali brojevi nisu dobri
 *  , u suprotnom vraca vrednost grupe karata
*/
int Group::isCorrectGroup(){


    bool allEqualSign = false;
    short counters[] = {0,0,0,0,0};
    bool allDifferentSign = true;

    if(cards.size() < 3)
        return -1; // VRACA -1 AKO GRUPA SADRZI MANJE OD 3 KARTE

    for(int i=0; i<cards.size(); i++)
    {
        switch(cards[i]->getSign()){
            case Card::KARO : counters[Card::KARO]++; break;
            case Card::HERC : counters[Card::HERC]++; break;
            case Card::PIK : counters[Card::PIK]++; break;
            case Card::TREF : counters[Card::TREF]++; break;
            default: counters[Card::JOKER]++; break;
        }
    }

    if(counters[Card::JOKER] > 1)
        return -2; // VRACA -2 AKO GRUPA SADRZI VISE OD 1 JOKERA

    for(int i=0; i<5; i++)
    {
        //PROVERA DA LI SU SVI ISTOG ZNAKA
        if(counters[i] == (int)cards.size() || (counters[i] == (int)(cards.size()-1) && counters[Card::JOKER]== 1))
        {
            allEqualSign = true;
            allDifferentSign = false;
            break;
        }
        if(allDifferentSign && counters[i]>1)
            allDifferentSign = false;

    }


    if(!allEqualSign && !allDifferentSign)
        return -3; // niti su istog znaka niti su svi razlicitog

   //sort stavlja jokera na pocetak jer mu je value 0 valjda
    correctValues();
    sortGroup();

    if(allDifferentSign)
    {
        for(int i=2; i<cards.size(); i++)
            if(cards[i]->getNumber() != cards[1]->getNumber())
                return -4;

        //ako je dobra grupa vraca vrednost grupe
        if(cards[1]->getNumber() == 1 || cards[1]->getNumber() > 10)
            return 10 * cards.size();
        else
            // mislim da ce ovde nesto morati da se menja
            //zato sto grupa ima konstantan broj pokazivaca za
            // sad, to sto neki pokazuju na NULL ova metoda
            //mislim da nece moci da prepozna
            return cards[1]->getValue() * cards.size();
    }

    //ako imamo 1 u grupi J Q K 1 sort ga stavlja na prvu poziciju,sto nije dobro
    //ovde menjamo vrednost toj karti ako je ima i sortiramo opet
    //OVDE DA SE PROMENI!!!
    int i = counters[Card::JOKER];

    if(cards[i]->getNumber() == 1 && (cards.last()->getNumber()==14 || cards.last()->getNumber() == 13) )
    {
        cards[i]->setValue(15);
        sortGroup();
    }


    if(allEqualSign)
    {
        //ako ima jokera ubacuje se na odgovarajucu poziciju
        if(counters[Card::JOKER] == 1 && cards[0]->getSign() == Card::JOKER)
            i = 1;
        else
            i = 0;
        for(i; i<cards.size()-1; i++)
        {
            int distance = cards[i+1]->getValue() - cards[i]->getValue();

            if( distance == 2)
            {
                if(cards[i]->getNumber()== 10 && cards[i+1]->getNumber() == 12)
                    continue;

                if(counters[Card::JOKER] == 0)
                    return -6;

                if(cards[0]->getSign() == Card::JOKER)
                {
                    Card* tmp = cards[0];
                    tmp->setValue(cards[i]->getValue()+1); //desilo mi se da sam izbacio grupu sa jokerom
                    cards.insert(i+1,tmp);                  // a kad sam je vratio u ruke joker mi se promenio
                    cards.pop_front();
                    counters[Card::JOKER] = 0;                              // u kartu koju je i sam menjao , promenila se slika! :D
                }
                else return -6;
            }
            else if( distance > 2)
            {
                if( (cards[i]->getNumber()== 9 && cards[i+1]->getNumber() == 12)
                        || (cards[i]->getNumber() == 10 && cards[i+1]->getNumber() == 13))
                {
                    if(counters[Card::JOKER] == 0)
                        return -6;

                    Card* tmp = cards[0];
                    tmp->setValue(cards[i]->getValue()+1); //desilo mi se da sam izbacio grupu sa jokerom
                    cards.insert(i+1,tmp);                  // a kad sam je vratio u ruke joker mi se promenio
                    cards.pop_front();
                    counters[Card::JOKER] = 0; // u kartu koju je i sam menjao , promenila se slika! :D
                }
                else
                    return -6;
            }
            else if( distance == 0 )
                return -6;
        }


        //ako ima jokera a ne postoji pozicija na koju  bi ga ubacili
        //stavljamo ga na najvisu poziciju u grupi i dodeljujemo vrednost
        if(counters[Card::JOKER] == 1 && i==cards.size()-1)
        {
            // ako je najvisa karta u grupi razlicita od A
            if(cards.last()->getValue() != 15)
            {
                Card* tmp = cards[0];
                tmp->setValue(cards.last()->getNumber()+1);
                cards.push_back(tmp);
                cards.pop_front();
            }
            // ako je najvisa karta u gurpi A
            // (posle A ne moze da stoji nista pa jokera ostavljamo na najnizoj poziciji,i dajemo mu vrednost)
            else
                cards[0]->setValue(cards[1]->getNumber()-1);
        }

        //trivijalno racunanje vrednosti grupe, ako grupa ne sadrzi jokera
        //zameni sa accumulate
        int sum=0;
        for(int i=0; i<cards.size(); i++)
            if(cards[i]->getValue()>=10)
                sum += 10;
            else
                sum += cards[i]->getValue();

        return sum;
    }
    return 0;
}
