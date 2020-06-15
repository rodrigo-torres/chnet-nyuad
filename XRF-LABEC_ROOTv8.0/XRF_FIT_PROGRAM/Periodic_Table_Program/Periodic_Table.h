#ifndef PERIODIC_TABLE_H
#define PERIODIC_TABLE_H

#include <QtWidgets>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox> 
#include <QCheckBox>
#include <QMenu>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QSizePolicy>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <QScreen>
#include <time.h>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDir>

class QLineEdit;
class QToolButton;


class Elemento : public QToolButton  ///ogni elemento è un bottone e ci può creare passandogli il nome (QString)
{
    Q_OBJECT

public:
    explicit Elemento(const QString &text, QWidget *parent = 0);
    void ChangeColor(const QPalette &);
    QSize sizeHint() const override;  //per sovrascrivere la regola delle dimensioni (rifatta nuova) 
};


class Periodic_Table : public QWidget
{
    Q_OBJECT

public:
    Periodic_Table(QWidget *parent = 0);
    ~ Periodic_Table();

private slots:
    void ElementClicked(QString , bool);
    void WriteEnergies1(float, float);
    void WriteEnergies2(float, float, float);
    void WriteEnergies3(float, float, float, float);
    void WriteEnergies4(float, float, float, float, float, float);
    void HClicked();
    void LiClicked();
    void NaClicked();
    void MgClicked();
    void SiClicked();
    void PClicked();
    void AlClicked();
    void SClicked();
    void ClClicked();
    void ArClicked();
    void KClicked();
    void CaClicked();
    void ScClicked();
    void TiClicked();
    void VClicked();
    void CrClicked();
    void MnClicked();
    void FeClicked();    
    void CoClicked();
    void NiClicked();
    void CuClicked();
    void ZnClicked();
    void GaClicked();
    void GeClicked();
    void AsClicked();
    void SeClicked();
    void BrClicked();
    void KrClicked();
    void RbClicked();
    void SrClicked();
    void YClicked();
    void ZrClicked();
    void NbClicked();
    void MoClicked();
    void TcClicked();
    void RuClicked();
    void RhClicked();
    void PdClicked();
    void AgClicked();
    void CdClicked();
    void InClicked();
    void SnClicked();
    void SbClicked();
    void TeClicked();
    void IClicked();
    void XeClicked();
    void CsClicked();
    void BaClicked();
    void HfClicked();
    void TaClicked();
    void WClicked();
    void ReClicked();
    void OsClicked();
    void IrClicked();
    void PtClicked();
    void AuClicked();
    void HgClicked();
    void TlClicked();
    void PbClicked();
    void BiClicked();
    void PoClicked();
    void AtClicked();
    void RnClicked();
    void FrClicked();
    void RaClicked();
    void BeClicked();
    void LuClicked();
    void LrClicked();
    void RfClicked();
    void DbClicked();
    void SgClicked();
    void BhClicked();
    void HsClicked();
    void MtClicked();
    void DsClicked();
    void RgClicked();
    void CnClicked();
    void BClicked();
    void CClicked();
    void UutClicked();
    void UupClicked();
    void UuoClicked();
    void FlClicked();
    void NClicked();
    void OClicked();
    void LvClicked();
    void FClicked();
    void HeClicked();
    void NeClicked();
private:
    QLineEdit *display;
    Elemento *createButton(const QString &text, const char *member);
    //enum { NumElements = 10 };
    //Elemento *Elements[NumElements];

};

#endif





