#include "Periodic_Table.h"
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <iostream>
#include <../Shm.h>
#include <../variables.h>
using std::cout;
using std::cin;
using std::endl;


bool found=false;
QString message;
QString s;
int caso=-1;

float *shm_fit,shmid_fit; 
key_t key_fit;
int SHMSZ_FIT=200;

bool HfOn=false,AlOn=false,SbOn=false,AgOn=false,ArOn=false,AsOn=false,AtOn=false,AcOn=false,NOn=false,BaOn=false,BeOn=false,BiOn=false,BOn=false,
BrOn=false,CdOn=false,CaOn=false,COn=false,CeOn=false,CsOn=false,ClOn=false,CoOn=false,KrOn=false,CrOn=false,DyOn=false,HeOn=false,ErOn=false,
EuOn=false,FeOn=false,FOn=false,POn=false,FrOn=false,GdOn=false,GaOn=false,GeOn=false,HOn=false,InOn=false,IOn=false,IrOn=false,YbOn=false,
YOn=false,LaOn=false,LiOn=false,LuOn=false,MgOn=false,MnOn=false,HgOn=false,MoOn=false,NdOn=false,NeOn=false,NiOn=false,NbOn=false,HoOn=false,
AuOn=false,OsOn=false,OOn=false,PdOn=false,PbOn=false,PtOn=false,PoOn=false,KOn=false,PrOn=false,PmOn=false,PaOn=false,RaOn=false,RnOn=false,
CuOn=false,ReOn=false,RhOn=false,RbOn=false,RuOn=false,SmOn=false,ScOn=false,SeOn=false,SiOn=false,NaOn=false,SnOn=false,SrOn=false,TlOn=false,
TaOn=false,TcOn=false,TeOn=false,TbOn=false,TiOn=false,ThOn=false,TmOn=false,WOn=false,UOn=false,VOn=false,XeOn=false,ZnOn=false,ZrOn=false,SOn=false;

Elemento::Elemento(const QString &text, QWidget *parent) //quando si crea si scrive sopra il suo nome (text)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    setText(text);
}

QSize Elemento::sizeHint() const  ///size ridefinita fissa
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}

void Elemento::ChangeColor(const QPalette & c)
{
setPalette(c);
}

Periodic_Table::Periodic_Table(QWidget *parent)
    : QWidget(parent)
{
 ////////////memoria condivisa//////////////////
   key = 6000;
   shmid_fit = shmget (key, SHMSZ_FIT, IPC_CREAT | 0666);
       if (shmid_fit == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shm_fit =(float *) shmat(shmid_fit, NULL, 0);
   cout << "Data Memory attached at :" << (float *)shm_fit<< endl << " with shmid: " << shmid_fit << endl;

///////posizioni usate
//shm  caso 
//shm+1 Ka
//shm+2 Kb
//shm+3 La
//shm+4 Lb
//shm+5 Ma
//shm+6 Mb
//shm+10 ---> nuove energie da controllare
//shm+11  was_already_clicked: vale 1 se l'elem era già cliccato, 0 se non lo era

//shm+19  stato della finestra Periodic_Table
//shm+20  pid della tavola periodica
    int pid=getpid();
    *(shm_fit+20)=pid;
/////////////////////

    display = new QLineEdit(" ");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    //display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 2);
    display->setFont(font);

/*///prova cambio colore...non funziona
	QPalette pal = HButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::blue));
        //HButton->setAutoFillBackground(true);
        HButton->setPalette(pal);
        HButton->update();
	*/
/////col 1
    Elemento *HButton = createButton(tr("H"), SLOT(HClicked()));  
    Elemento *LiButton = createButton(tr("Li"), SLOT(LiClicked()));
    Elemento *NaButton = createButton(tr("Na"), SLOT(NaClicked()));
    Elemento *KButton = createButton(tr("K"), SLOT(KClicked()));
    Elemento *RbButton = createButton(tr("Rb"), SLOT(RbClicked()));
    Elemento *CsButton = createButton(tr("Cs"), SLOT(CsClicked()));
    Elemento *FrButton = createButton(tr("Fr"), SLOT(FrClicked()));
////col 2
    Elemento *BeButton = createButton(tr("Be"), SLOT(BeClicked()));
    Elemento *MgButton = createButton(tr("Mg"), SLOT(MgClicked()));
    Elemento *CaButton = createButton(tr("Ca"), SLOT(CaClicked()));
    Elemento *SrButton = createButton(tr("Sr"), SLOT(SrClicked()));
    Elemento *BaButton = createButton(tr("Ba"), SLOT(BaClicked()));
    Elemento *RaButton = createButton(tr("Ra"), SLOT(RaClicked()));
////col 3
    Elemento *ScButton = createButton(tr("Sc"), SLOT(ScClicked()));
    Elemento *YButton = createButton(tr("Y"), SLOT(YClicked()));
    Elemento *LuButton = createButton(tr("Lu"), SLOT(LuClicked()));
    Elemento *LrButton = createButton(tr("Lr"), SLOT(LrClicked()));
////col 4
    Elemento *TiButton = createButton(tr("Ti"), SLOT(TiClicked()));
    Elemento *ZrButton = createButton(tr("Zr"), SLOT(ZrClicked()));
    Elemento *HfButton = createButton(tr("Hf"), SLOT(HfClicked()));
    Elemento *RfButton = createButton(tr("Rf"), SLOT(RfClicked()));
////col 5
    Elemento *VButton = createButton(tr("V"), SLOT(VClicked()));
    Elemento *NbButton = createButton(tr("Nb"), SLOT(NbClicked()));
    Elemento *TaButton = createButton(tr("Ta"), SLOT(TaClicked()));
    Elemento *DbButton = createButton(tr("Db"), SLOT(DbClicked()));
////col 6
    Elemento *CrButton = createButton(tr("Cr"), SLOT(CrClicked()));
    Elemento *MoButton = createButton(tr("Mo"), SLOT(MoClicked()));
    Elemento *WButton = createButton(tr("W"), SLOT(WClicked()));
    Elemento *SgButton = createButton(tr("Sg"), SLOT(SgClicked()));
////col 7
    Elemento *MnButton = createButton(tr("Mn"), SLOT(MnClicked()));
    Elemento *TcButton = createButton(tr("Tc"), SLOT(TcClicked()));
    Elemento *ReButton = createButton(tr("Re"), SLOT(ReClicked()));
    Elemento *BhButton = createButton(tr("Bh"), SLOT(BhClicked()));
////col 8
    Elemento *FeButton = createButton(tr("Fe"), SLOT(FeClicked()));
    Elemento *RuButton = createButton(tr("Ru"), SLOT(RuClicked()));
    Elemento *OsButton = createButton(tr("Os"), SLOT(OsClicked()));
    Elemento *HsButton = createButton(tr("Hs"), SLOT(HsClicked()));
////col 9
    Elemento *CoButton = createButton(tr("Co"), SLOT(CoClicked()));
    Elemento *RhButton = createButton(tr("Rh"), SLOT(RhClicked()));
    Elemento *IrButton = createButton(tr("Ir"), SLOT(IrClicked()));
    Elemento *MtButton = createButton(tr("Mt"), SLOT(MtClicked()));
////col 10
    Elemento *NiButton = createButton(tr("Ni"), SLOT(NiClicked()));
    Elemento *PdButton = createButton(tr("Pd"), SLOT(PdClicked()));
    Elemento *PtButton = createButton(tr("Pt"), SLOT(PtClicked()));
    Elemento *DsButton = createButton(tr("Ds"), SLOT(DsClicked()));
////col 11
    Elemento *CuButton = createButton(tr("Cu"), SLOT(CuClicked()));
    Elemento *AgButton = createButton(tr("Ag"), SLOT(AgClicked()));
    Elemento *AuButton = createButton(tr("Au"), SLOT(AuClicked()));
    Elemento *RgButton = createButton(tr("Rg"), SLOT(RgClicked()));
////col 12
    Elemento *ZnButton = createButton(tr("Zn"), SLOT(ZnClicked()));
    Elemento *CdButton = createButton(tr("Cd"), SLOT(CdClicked()));
    Elemento *HgButton = createButton(tr("Hg"), SLOT(HgClicked()));
    Elemento *CnButton = createButton(tr("Cn"), SLOT(CnClicked()));
////col 13
    Elemento *BButton = createButton(tr("B"), SLOT(BClicked()));
    Elemento *AlButton = createButton(tr("Al"), SLOT(AlClicked()));
    Elemento *GaButton = createButton(tr("Ga"), SLOT(GaClicked()));
    Elemento *InButton = createButton(tr("In"), SLOT(InClicked()));
    Elemento *TlButton = createButton(tr("Tl"), SLOT(TlClicked()));
    Elemento *UutButton = createButton(tr("Uut"), SLOT(UutClicked()));
////col 14
    Elemento *CButton = createButton(tr("C"), SLOT(CClicked()));
    Elemento *SiButton = createButton(tr("Si"), SLOT(SiClicked()));
    Elemento *GeButton = createButton(tr("Ge"), SLOT(GeClicked()));
    Elemento *SnButton = createButton(tr("Sn"), SLOT(SnClicked()));
    Elemento *PbButton = createButton(tr("Pb"), SLOT(PbClicked()));
    Elemento *FlButton = createButton(tr("Fl"), SLOT(FlClicked()));
////col 15
    Elemento *NButton = createButton(tr("N"), SLOT(NClicked()));
    Elemento *PButton = createButton(tr("P"), SLOT(PClicked()));
    Elemento *AsButton = createButton(tr("As"), SLOT(AsClicked()));
    Elemento *SbButton = createButton(tr("Sb"), SLOT(SbClicked()));
    Elemento *BiButton = createButton(tr("Bi"), SLOT(BiClicked()));
    Elemento *UupButton = createButton(tr("Uup"), SLOT(UupClicked()));
////col 16
    Elemento *OButton = createButton(tr("O"), SLOT(OClicked()));
    Elemento *SButton = createButton(tr("S"), SLOT(SClicked()));
    Elemento *SeButton = createButton(tr("Se"), SLOT(SeClicked()));
    Elemento *TeButton = createButton(tr("Te"), SLOT(TeClicked()));
    Elemento *PoButton = createButton(tr("Po"), SLOT(PoClicked()));
    Elemento *LvButton = createButton(tr("Lv"), SLOT(LvClicked()));
////col 17
    Elemento *FButton = createButton(tr("F"), SLOT(FClicked()));
    Elemento *ClButton = createButton(tr("Cl"), SLOT(ClClicked()));
    Elemento *BrButton = createButton(tr("Br"), SLOT(BrClicked()));
    Elemento *IButton = createButton(tr("I"), SLOT(IClicked()));
    Elemento *AtButton = createButton(tr("At"), SLOT(AtClicked()));
    Elemento *UusButton = createButton(tr("Uus"), SLOT(UutClicked()));
////col 18
    Elemento *HeButton = createButton(tr("He"), SLOT(HeClicked()));
    Elemento *NeButton = createButton(tr("Ne"), SLOT(NeClicked()));
    Elemento *ArButton = createButton(tr("Ar"), SLOT(ArClicked()));
    Elemento *KrButton = createButton(tr("Kr"), SLOT(KrClicked()));
    Elemento *XeButton = createButton(tr("Xe"), SLOT(XeClicked()));
    Elemento *RnButton = createButton(tr("Rn"), SLOT(RnClicked()));
    Elemento *UuoButton = createButton(tr("Uuo"), SLOT(UuoClicked()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(display, 0, 1, 1, 16);

    mainLayout->addWidget(HButton, 1, 0);     mainLayout->addWidget(BeButton, 2, 1);
    mainLayout->addWidget(LiButton, 2, 0);    mainLayout->addWidget(MgButton, 3, 1);
    mainLayout->addWidget(NaButton, 3, 0);    mainLayout->addWidget(CaButton, 4, 1);
    mainLayout->addWidget(KButton, 4, 0);    mainLayout->addWidget(SrButton, 5, 1);
    mainLayout->addWidget(RbButton, 5, 0);    mainLayout->addWidget(BaButton, 6, 1);
    mainLayout->addWidget(CsButton, 6, 0);    mainLayout->addWidget(RaButton, 7, 1);
    mainLayout->addWidget(FrButton, 7, 0);

    mainLayout->addWidget(ScButton, 4, 2);    mainLayout->addWidget(TiButton, 4, 3);   
    mainLayout->addWidget(YButton, 5, 2);     mainLayout->addWidget(ZrButton, 5, 3);   
    mainLayout->addWidget(LuButton, 6, 2);    mainLayout->addWidget(HfButton, 6, 3);   
    mainLayout->addWidget(LrButton, 7, 2);    mainLayout->addWidget(RfButton, 7, 3);   

    mainLayout->addWidget(VButton, 4, 4);    mainLayout->addWidget(CrButton, 4, 5);   
    mainLayout->addWidget(NbButton, 5, 4);     mainLayout->addWidget(MoButton, 5, 5);   
    mainLayout->addWidget(TaButton, 6, 4);    mainLayout->addWidget(WButton, 6, 5);   
    mainLayout->addWidget(DbButton, 7, 4);    mainLayout->addWidget(SgButton, 7, 5);    

    mainLayout->addWidget(MnButton, 4, 6);    mainLayout->addWidget(FeButton, 4,7);   
    mainLayout->addWidget(TcButton, 5, 6);     mainLayout->addWidget(RuButton, 5, 7);   
    mainLayout->addWidget(ReButton, 6, 6);    mainLayout->addWidget(OsButton, 6, 7);   
    mainLayout->addWidget(BhButton, 7, 6);    mainLayout->addWidget(HsButton, 7, 7);

    mainLayout->addWidget(CoButton, 4, 8);    mainLayout->addWidget(NiButton, 4,9);   
    mainLayout->addWidget(RhButton, 5, 8);     mainLayout->addWidget(PdButton, 5, 9);   
    mainLayout->addWidget(IrButton, 6, 8);    mainLayout->addWidget(PtButton, 6, 9);   
    mainLayout->addWidget(MtButton, 7, 8);    mainLayout->addWidget(DsButton, 7, 9);

    mainLayout->addWidget(CuButton, 4, 10);    mainLayout->addWidget(ZnButton, 4,11);   
    mainLayout->addWidget(AgButton, 5, 10);     mainLayout->addWidget(CdButton, 5, 11);   
    mainLayout->addWidget(AuButton, 6, 10);    mainLayout->addWidget(HgButton, 6, 11);   
    mainLayout->addWidget(RgButton, 7, 10);    mainLayout->addWidget(CnButton, 7, 11);

    mainLayout->addWidget(BButton, 2, 12);     mainLayout->addWidget(CButton, 2, 13);
    mainLayout->addWidget(AlButton, 3, 12);    mainLayout->addWidget(SiButton, 3, 13);
    mainLayout->addWidget(GaButton, 4, 12);    mainLayout->addWidget(GeButton, 4, 13);
    mainLayout->addWidget(InButton, 5, 12);    mainLayout->addWidget(SnButton, 5, 13);
    mainLayout->addWidget(TlButton, 6, 12);    mainLayout->addWidget(PbButton, 6, 13);
    mainLayout->addWidget(UutButton, 7, 12);    mainLayout->addWidget(FlButton, 7, 13);

    mainLayout->addWidget(NButton, 2, 14);     mainLayout->addWidget(OButton, 2, 15);
    mainLayout->addWidget(PButton, 3, 14);    mainLayout->addWidget(SButton, 3, 15);
    mainLayout->addWidget(AsButton, 4, 14);    mainLayout->addWidget(SeButton, 4, 15);
    mainLayout->addWidget(SbButton, 5, 14);    mainLayout->addWidget(TeButton, 5, 15);
    mainLayout->addWidget(BiButton, 6, 14);    mainLayout->addWidget(PoButton, 6, 15);
    mainLayout->addWidget(UupButton, 7, 14);    mainLayout->addWidget(LvButton, 7, 15);

    mainLayout->addWidget(FButton, 2, 16);     mainLayout->addWidget(HeButton, 1, 17);
    mainLayout->addWidget(ClButton, 3, 16);    mainLayout->addWidget(NeButton, 2, 17);
    mainLayout->addWidget(BrButton, 4, 16);    mainLayout->addWidget(ArButton, 3, 17);
    mainLayout->addWidget(IButton, 5, 16);    mainLayout->addWidget(KrButton, 4, 17);
    mainLayout->addWidget(AtButton, 6, 16);    mainLayout->addWidget(XeButton, 5, 17);
    mainLayout->addWidget(UusButton, 7, 16);    mainLayout->addWidget(RnButton, 6, 17); mainLayout->addWidget(UuoButton, 7, 17);
    setLayout(mainLayout);

    setWindowTitle(tr("Periodic Table"));
}

Elemento *Periodic_Table::createButton(const QString &text, const char *member)
{
    Elemento *button = new Elemento(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

/////////////////////////////////
//se si clicca un elemento lo cerca nel file e scrive in memoria le energie delle righe e lo "stato" dell'elemento (was_already_clicked)
void Periodic_Table::ElementClicked(QString name, bool was_already_clicked) 
{
if(was_already_clicked) *(shm_fit+11)=0; //era già cliccato, si devono cancellare i marker
else *(shm_fit+11)=1; //non era cliccato, si devono disegnare i marker
found=false;
QFile file("XrayEnergy_data"); 
file.open(QIODevice::ReadOnly);
QString line;
  while( !file.atEnd() && !found) 
    {
          line = file.readLine();
          if(line.contains(name, Qt::CaseInsensitive))
           {
            QStringList pieces = line.split( " " );
            switch (caso)
            {
            case(1):
             {
               QString e1 = pieces.value( pieces.length() - 2 );  //Ka
               QString e2 = pieces.value( pieces.length()-1); //Kb
               message="Ka: ";
               message.append(e1);
               message.append(" keV  Kb: ");
               message.append(e2);
               message.append(" keV");
               WriteEnergies1(e1.toFloat(),e2.toFloat());
               break;
             }
            case(2):
             {
               QString e1 = pieces.value( pieces.length() - 3 );  //Ka
               QString e2 = pieces.value( pieces.length()-2); //Kb
               QString e3 = pieces.value( pieces.length()-1); //La
               message="Ka: ";
               message.append(e1);
               message.append(" keV  Kb: ");
               message.append(e2);
               message.append(" keV  La: ");
               message.append(e3);
               message.append(" keV");
               WriteEnergies2(e1.toFloat(),e2.toFloat(),e3.toFloat());
               break;
             }
            case(3):
             {
               QString e1 = pieces.value( pieces.length() - 4 );  //Ka
               QString e2 = pieces.value( pieces.length()-3); //Kb
               QString e3 = pieces.value( pieces.length()-2); //La
               QString e4 = pieces.value( pieces.length()-1); //Lb
               message="Ka: ";
               message.append(e1);
               message.append(" keV  Kb: ");
               message.append(e2);
               message.append(" keV  La: ");
               message.append(e3);
               message.append(" keV  Lb: ");
               message.append(e4);
               message.append(" keV");
               WriteEnergies3(e1.toFloat(),e2.toFloat(),e3.toFloat(),e4.toFloat());
               break;
             }
            case(4):
             {
               QString e1 = pieces.value( pieces.length() -6 );  //Ka
               QString e2 = pieces.value( pieces.length()-5); //Kb
               QString e3 = pieces.value( pieces.length()-4); //La
               QString e4 = pieces.value( pieces.length()-3); //Lb
               QString e5 = pieces.value( pieces.length()-2); //Ma
               QString e6 = pieces.value( pieces.length()-1); //Mb
               message="Ka: ";
               message.append(e1);
               message.append(" keV  Kb: ");
               message.append(e2);
               message.append(" keV  La: ");
               message.append(e3);
               message.append(" keV  Lb: ");
               message.append(e4);
               message.append(" keV  Ma: ");
               message.append(e5);
               message.append(" keV  Mb: ");
               message.append(e6);
               message.append(" keV");
               WriteEnergies4(e1.toFloat(),e2.toFloat(),e3.toFloat(),e4.toFloat(),e5.toFloat(),e6.toFloat());
               break;
             }
            }

            display->setText(message);
            //qDebug()<<e1<<e2;
            found=true;
           }
    }
file.close();
}


void Periodic_Table::WriteEnergies1(float en1, float en2)
{
*(shm_fit)=1; //caso
*(shm_fit+1)=en1; //Ka
*(shm_fit+2)=en2; //Kb
*(shm_fit+10)=1;
//qDebug()<<*(shm_fit)<<*(shm_fit+1)<<*(shm_fit+2);
}

void Periodic_Table::WriteEnergies2(float en1, float en2, float en3)
{
*(shm_fit)=2; //caso
*(shm_fit+1)=en1; //Ka
*(shm_fit+2)=en2; //Kb
*(shm_fit+3)=en3; //La
*(shm_fit+10)=1;
}

void Periodic_Table::WriteEnergies3(float en1, float en2, float en3, float en4)
{
*(shm_fit)=3; //caso
*(shm_fit+1)=en1; //Ka
*(shm_fit+2)=en2; //Kb
*(shm_fit+3)=en3; //La
*(shm_fit+4)=en4; //Lb
*(shm_fit+10)=1;
}

void Periodic_Table::WriteEnergies4(float en1, float en2, float en3, float en4,  float en5, float en6)
{
*(shm_fit)=4; //caso
*(shm_fit+1)=en1; //Ka
*(shm_fit+2)=en2; //Kb
*(shm_fit+3)=en3; //La
*(shm_fit+4)=en4; //Lb
*(shm_fit+5)=en5; //Ma
*(shm_fit+6)=en6; //Mb
*(shm_fit+10)=1;
}

void Periodic_Table::HClicked()
{
qDebug()<<"H clicked\n";
}
void Periodic_Table::BeClicked()
{
qDebug()<<"Be clicked\n";
}
void Periodic_Table::LuClicked()
{
qDebug()<<"Lu clicked\n";
}
void Periodic_Table::LrClicked()
{
qDebug()<<"Lr clicked\n";
}

void Periodic_Table::RfClicked()
{
qDebug()<<"Rf clicked\n";
}
void Periodic_Table::DbClicked()
{
qDebug()<<"Db clicked\n";
}
void Periodic_Table::SgClicked()
{
qDebug()<<"Sg clicked\n";
}
void Periodic_Table::BhClicked()
{
qDebug()<<"Bh clicked\n";
}
void Periodic_Table::HsClicked()
{
qDebug()<<"Hs clicked\n";
}
void Periodic_Table::MtClicked()
{
qDebug()<<"Mt clicked\n";
}
void Periodic_Table::DsClicked()
{
qDebug()<<"Ds clicked\n";
}
void Periodic_Table::RgClicked()
{
qDebug()<<"Rg clicked\n";
}
void Periodic_Table::CnClicked()
{
qDebug()<<"Cn clicked\n";
}
void Periodic_Table::BClicked()
{
qDebug()<<"B clicked\n";
}
void Periodic_Table::CClicked()
{
qDebug()<<"C clicked\n";
}
void Periodic_Table::UutClicked()
{
qDebug()<<"Uut clicked\n";
}
void Periodic_Table::FlClicked()
{
qDebug()<<"Fl clicked\n";
}
void Periodic_Table::NClicked()
{
qDebug()<<"N clicked\n";
}
void Periodic_Table::UupClicked()
{
qDebug()<<"Uup clicked\n";
}
void Periodic_Table::OClicked()
{
qDebug()<<"O clicked\n";
}
void Periodic_Table::LvClicked()
{
qDebug()<<"Lv clicked\n";
}
void Periodic_Table::FClicked()
{
qDebug()<<"F clicked\n";
}
void Periodic_Table::UuoClicked()
{
qDebug()<<"Uuo clicked\n";
}
void Periodic_Table::NeClicked()
{
qDebug()<<"Ne clicked\n";
}
void Periodic_Table::HeClicked()
{
qDebug()<<"He clicked\n";
}
/////////////////
void Periodic_Table::NaClicked()
{
caso=1;
if(NaOn) NaOn=false; 
else NaOn=true;
ElementClicked("Na",NaOn);
}

void Periodic_Table::LiClicked()
{
qDebug()<<"Li clicked\n";
}
void Periodic_Table::MgClicked()
{
caso=1;
if(MgOn) MgOn=false; 
else MgOn=true;
ElementClicked("Mg",MgOn);
}
void Periodic_Table::AlClicked()
{
caso=1;
if(AlOn) AlOn=false; 
else AlOn=true;
ElementClicked("Al",AlOn);
}
void Periodic_Table::SiClicked()
{
caso=1;
if(SiOn) SiOn=false; 
else SiOn=true;
ElementClicked("Si",SiOn);
}
void Periodic_Table::PClicked()
{
caso=1;
if(POn) POn=false; 
else POn=true;
ElementClicked("P",POn);
}
void Periodic_Table::SClicked()
{
caso=1;
if(SOn) SOn=false; 
else SOn=true;
ElementClicked("S",SOn);
}
void Periodic_Table::ClClicked()
{
caso=1;
if(ClOn) ClOn=false; 
else ClOn=true;
ElementClicked("Cl",ClOn);
}
void Periodic_Table::ArClicked()
{
caso=1;
if(ArOn) ArOn=false; 
else ArOn=true;
ElementClicked("Ar",ArOn);
}
void Periodic_Table::KClicked()
{
caso=1;
if(KOn) KOn=false; 
else KOn=true;
ElementClicked("K ",KOn);
}
//////////////
void Periodic_Table::CaClicked()
{
caso=2;
if(CaOn) CaOn=false; 
else CaOn=true;
ElementClicked("Ca",CaOn);
}
void Periodic_Table::ScClicked()
{
caso=2;
if(ScOn) ScOn=false; 
else ScOn=true;
ElementClicked("Sc",ScOn);
}
//////////////////
void Periodic_Table::TiClicked()
{
caso=3;
if(TiOn) TiOn=false; 
else TiOn=true;
ElementClicked("Ti",TiOn);
}
void Periodic_Table::VClicked()
{
caso=3;
if(VOn) VOn=false; 
else VOn=true;
ElementClicked("V",VOn);
}
void Periodic_Table::CrClicked()
{
caso=3;
if(CrOn) CrOn=false; 
else CrOn=true;
ElementClicked("Cr",CrOn);
}
void Periodic_Table::MnClicked()
{
caso=3;
if(MnOn) MnOn=false; 
else MnOn=true;
ElementClicked("Mn",MnOn);
}
void Periodic_Table::FeClicked()
{
caso=3;
if(FeOn) FeOn=false; 
else FeOn=true;
ElementClicked("Fe",FeOn);
}
void Periodic_Table::CoClicked()
{
caso=3;
if(CoOn) CoOn=false; 
else CoOn=true;
ElementClicked("Co",CoOn);
}
void Periodic_Table::NiClicked()
{
caso=3;
if(NiOn) NiOn=false; 
else NiOn=true;
ElementClicked("Ni",NiOn);
}
void Periodic_Table::CuClicked()
{
caso=3;
if(CuOn) CuOn=false; 
else CuOn=true;
ElementClicked("Cu",CuOn);
}
void Periodic_Table::ZnClicked()
{
caso=3;
if(ZnOn) ZnOn=false; 
else ZnOn=true;
ElementClicked("Zn",ZnOn);
}
void Periodic_Table::GaClicked()
{
caso=3;
if(GaOn) GaOn=false; 
else GaOn=true;
ElementClicked("Ga",GaOn);
}
void Periodic_Table::GeClicked()
{
caso=3;
if(GeOn) GeOn=false; 
else GeOn=true;
ElementClicked("Ge",GeOn);
}
void Periodic_Table::AsClicked()
{
caso=3;
if(AsOn) AsOn=false; 
else AsOn=true;
ElementClicked("As",AsOn);
}
void Periodic_Table::SeClicked()
{
caso=3;
if(SeOn) SeOn=false; 
else SeOn=true;
ElementClicked("Se",SeOn);
}
void Periodic_Table::BrClicked()
{
caso=3;
if(BrOn) BrOn=false; 
else BrOn=true;
ElementClicked("Br",BrOn);
}
void Periodic_Table::KrClicked()
{
caso=3;
if(KrOn) KrOn=false; 
else KrOn=true;
ElementClicked("Kr",KrOn);
}
void Periodic_Table::RbClicked()
{
caso=3;
if(RbOn) RbOn=false; 
else RbOn=true;
ElementClicked("Rb",RbOn);
}
void Periodic_Table::SrClicked()
{
caso=3;
if(SrOn) SrOn=false; 
else SrOn=true;
ElementClicked("Sr",SrOn);
}
void Periodic_Table::YClicked()
{
caso=3;
if(YOn) YOn=false; 
else YOn=true;
ElementClicked("Y",YOn);
}
void Periodic_Table::ZrClicked()
{
caso=3;
if(ZrOn) ZrOn=false; 
else ZrOn=true;
ElementClicked("Zr",ZrOn);
}
void Periodic_Table::NbClicked()
{
caso=3;
if(NbOn) NbOn=false; 
else NbOn=true;
ElementClicked("Nb",NbOn);
}
void Periodic_Table::MoClicked()
{
caso=3;
if(MoOn) MoOn=false; 
else MoOn=true;
ElementClicked("Mo",MoOn);
}
void Periodic_Table::TcClicked()
{
caso=3;
if(TcOn) TcOn=false; 
else TcOn=true;
ElementClicked("Tc",TcOn);
}
void Periodic_Table::RuClicked()
{
caso=3;
if(RuOn) RuOn=false; 
else RuOn=true;
ElementClicked("Ru",RuOn);
}
void Periodic_Table::RhClicked()
{
caso=3;
if(RhOn) RhOn=false; 
else RhOn=true;
ElementClicked("Rh",RhOn);
}
void Periodic_Table::PdClicked()
{
caso=3;
if(PdOn) PdOn=false; 
else PdOn=true;
ElementClicked("Pd",PdOn);
}
void Periodic_Table::AgClicked()
{
caso=3;
if(AgOn) AgOn=false; 
else AgOn=true;
ElementClicked("Ag",AgOn);
}
void Periodic_Table::CdClicked()
{
caso=3;
if(CdOn) CdOn=false; 
else CdOn=true;
ElementClicked("Cd",CdOn);
}
void Periodic_Table::InClicked()
{
caso=3;
if(InOn) InOn=false; 
else InOn=true;
ElementClicked("In",InOn);
}
void Periodic_Table::SnClicked()
{
caso=3;
if(SnOn) SnOn=false; 
else SnOn=true;
ElementClicked("Sn",SnOn);
}
void Periodic_Table::SbClicked()
{
caso=3;
if(SbOn) SbOn=false; 
else SbOn=true;
ElementClicked("Sb",SbOn);
}
void Periodic_Table::TeClicked()
{
caso=3;
if(TeOn) TeOn=false; 
else TeOn=true;
ElementClicked("Te",TeOn);
}
void Periodic_Table::IClicked()
{
caso=3;
if(IOn) IOn=false; 
else IOn=true;
ElementClicked("I",IOn);
}
void Periodic_Table::XeClicked()
{
caso=3;
if(XeOn) XeOn=false; 
else XeOn=true;
ElementClicked("Xe",XeOn);
}
void Periodic_Table::CsClicked()
{
caso=3;
if(CsOn) CsOn=false; 
else CsOn=true;
ElementClicked("Cs",CsOn);
}
void Periodic_Table::BaClicked()
{
caso=3;
if(BaOn) BaOn=false; 
else BaOn=true;
ElementClicked("Ba",BaOn);
}
/////////////////////////////////////////////
void Periodic_Table::HfClicked()
{
caso=4;
if(HfOn) HfOn=false; 
else HfOn=true;
ElementClicked("Hf",HfOn);
}
void Periodic_Table::TaClicked()
{
caso=4;
if(TaOn) TaOn=false; 
else TaOn=true;
ElementClicked("Ta",TaOn);
}
void Periodic_Table::WClicked()
{
caso=4;
if(WOn) WOn=false; 
else WOn=true;
ElementClicked("W",WOn);
}
void Periodic_Table::ReClicked()
{
caso=4;
if(ReOn) ReOn=false; 
else ReOn=true;
ElementClicked("Re",ReOn);
}
void Periodic_Table::OsClicked()
{
caso=4;
if(OsOn) OsOn=false; 
else OsOn=true;
ElementClicked("Os",OsOn);
}
void Periodic_Table::IrClicked()
{
caso=4;
if(IrOn) IrOn=false; 
else IrOn=true;
ElementClicked("Ir",IrOn);
}
void Periodic_Table::PtClicked()
{
caso=4;
if(PtOn) PtOn=false; 
else PtOn=true;
ElementClicked("Pt",PtOn);
}
void Periodic_Table::AuClicked()
{
caso=4;
if(AuOn) AuOn=false; 
else AuOn=true;
ElementClicked("Au",AuOn);
}
void Periodic_Table::HgClicked()
{
caso=4;
if(HgOn) HgOn=false; 
else HgOn=true;
ElementClicked("Hg",HgOn);
}
void Periodic_Table::TlClicked()
{
caso=4;
if(TlOn) TlOn=false; 
else TlOn=true;
ElementClicked("Tl",TlOn);
}
void Periodic_Table::PbClicked()
{
caso=4;
if(PbOn) PbOn=false; 
else PbOn=true;
ElementClicked("Pb",PbOn);
}
void Periodic_Table::BiClicked()
{
caso=4;
if(BiOn) BiOn=false; 
else BiOn=true;
ElementClicked("Bi",BiOn);
}
void Periodic_Table::PoClicked()
{
caso=4;
if(PoOn) PoOn=false; 
else PoOn=true;
ElementClicked("Po",PoOn);
}
void Periodic_Table::AtClicked()
{
caso=4;
if(AtOn) AtOn=false; 
else AtOn=true;
ElementClicked("At",AtOn);
}
void Periodic_Table::RnClicked()
{
caso=4;
if(RnOn) RnOn=false; 
else RnOn=true;
ElementClicked("Rn",RnOn);
}
void Periodic_Table::FrClicked()
{
caso=4;
if(FrOn) FrOn=false; 
else FrOn=true;
ElementClicked("Fr",FrOn);
}
void Periodic_Table::RaClicked()
{
caso=4;
if(RaOn) RaOn=false; 
else RaOn=true;
ElementClicked("Ra",RaOn);
}


Periodic_Table::~Periodic_Table()
{
*(shm_fit+19)=0;
}
