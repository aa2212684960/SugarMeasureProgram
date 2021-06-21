#ifndef SUGARMEASURE_H
#define SUGARMEASURE_H


#include <QVector>
#include <QMap>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include <QWidget>

#define COTTROL Input_Temperature->value()!=20&&Output_TemperatureCorrect->value() || Input_Temperature->value()==20&&!Output_TemperatureCorrect->value()

class SugarMeasure:public QWidget
{
    Q_OBJECT
public:
    SugarMeasure(QWidget *parent = nullptr);
    ~SugarMeasure();
protected:
    void Ini_All();
    void Ini_Data();
    void Ini_Correct_Table();
    void Ini_Brix_ApparentDensity_Table();
    void Ini_Widget();
    void Ini_Layout();
    void Ini_Connect();
    void Ini_InputConnect();
    void Ini_OutputConnect();
    void Ini_ButtonConnect();
private:
    QVector<QVector<float>>* Brix_Correct_Table;//糖液观测锤度温度改正表(20℃)
    /*糖液锤度与视密度对照表*/
    QVector<float>* Brix_Vector;//锤度值Brix_Vector
    QVector<float>* ApparentDensity_Vector;//视密度ApparentDensity_Vector
    QMap<float,float>* Brix_Transform_BtoD_Table;//锤度转视密度
    QMap<float,float>* Brix_Transform_DtoB_Table;//视密度转锤度
private:
    QDoubleSpinBox *Input_Brix;//糖锤度输入框Input_Brix
    QDoubleSpinBox *Input_ApparentDensity;//视密度输入框Input_ApparentDensity
    QDoubleSpinBox *Output_g_Value;//g值输出框Output_g_Value
    QDoubleSpinBox *Output_Correct_Pol;//二次旋光法校正转光度输出框Output_Correct_Pol
    QDoubleSpinBox *Input_Sugar;//蔗糖分输出框Input_Sugar

    QSpinBox *Input_Temperature;//温度输入框Input_Temperature
    QSpinBox *Input_ObservedBrix;//观测锤度输入框Input_ObservedBrix
    QDoubleSpinBox *Input_KLJDivisorofTemperature;//克来杰除数对应温度输入框Input_KLJDivisorofTemperature

    QDoubleSpinBox *Output_TemperatureCorrect;//温度修正值输出框Output_TemperatureCorrect
    QDoubleSpinBox *Output_KLJDivisorofTemperatureCorrect;//KLJ除数对应温度更正值输出框Output_KLJDivisorofTemperatureCorrect

    QDoubleSpinBox *Output_DirectPol;//半值直接转光度Output_ DirectPol
    QDoubleSpinBox *Output_Transformpol;// 半值转化转光度Output_Transformpol
    QPushButton *PB_Determine;//PB_Determine
    QPushButton *PB_CalculateResult;//PB_CalculateResult
    QPushButton *PB_ClearData;//PB_ClearData

    QRadioButton *setArrow;//控制是否旋转框显示上下箭头
private:
    float BeforeValue;
    int JuiceTemperature;//蔗汁温度
    float JuiceTemperature_KLJ;//用来求克拉姐修正数时的蔗汁温度
    float ObservedBrix;
    float DirectPol;
    float TransformPol;
    float g_value;
    float KLJ_CorrectValue;
    float S_value;
    bool bDeterminePress;
public slots:
    void GetBrixforshow(double AD);
    void GetApparentDensityforshow(double Brix);
    void SaveTemperature(int  JuiceTemperature){ JuiceTemperature = JuiceTemperature; }//获取温度修正值并显示
    void SaveObservedBrix(int Brix){ ObservedBrix = Brix; }//保存蔗汁温度
    void SaveTemperature_KLJ(double JuiceTemperature_KLJ){ this->JuiceTemperature_KLJ = JuiceTemperature_KLJ; }
    void SaveDirectPol(double pol){ DirectPol = pol; }
    void SaveTransformPol(double pol){ TransformPol = -pol; }
    void SetbDeterminePress(){ bDeterminePress = true; }
    void GetBrixCorrection();
    void CalculateKLJ_CorrectValue();//Cor=Correct
    void Calculate_g();//计算g值
    void Calculate_S();
    void Finalresult();
    void ClearData();

    void ArrowShoworHide(bool bshow);
    void SetCorrectBrix(double Correctvalue);
};
#endif // SugarMeasure_H
