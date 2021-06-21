#include "SugarMeasure.h"
SugarMeasure::SugarMeasure(QWidget *parent):QWidget(parent)
{
    Ini_All();
}

SugarMeasure::~SugarMeasure()
{
    Brix_Vector->clear();//使size为0
    QVector<float>().swap(*Brix_Vector);//使得capacity为0
    ApparentDensity_Vector->clear();
    QVector<float>().swap(*ApparentDensity_Vector);
    Brix_Correct_Table->clear();
    QVector<QVector<float>>().swap(*Brix_Correct_Table);
    QMap<float,float>().swap(*Brix_Transform_BtoD_Table);//释放QMap的内存
    QMap<float,float>().swap(*Brix_Transform_DtoB_Table);
}

void SugarMeasure::Ini_All()
{
    Ini_Correct_Table();
    Ini_Brix_ApparentDensity_Table();
    Ini_Widget();
    Ini_Layout();
    Ini_Data();
    Ini_Connect();
}

void SugarMeasure::Ini_Data()
{
    JuiceTemperature = 10;
    ObservedBrix = 11;
    bDeterminePress = false;
    Output_TemperatureCorrect->setValue(0); Output_TemperatureCorrect->setPrefix(" ");
    Output_KLJDivisorofTemperatureCorrect->setValue(0);
    Output_DirectPol->setValue(0); Output_Transformpol->setValue(0);
    Output_DirectPol->clear(); Output_Transformpol->clear();
    Output_g_Value->setValue(0); Output_Correct_Pol->setValue(0); Input_Sugar->setValue(0);
    Input_Temperature->clear(); Input_ObservedBrix->clear(); Input_KLJDivisorofTemperature->setValue(0); Input_KLJDivisorofTemperature->clear();
   // Output_KLJDivisorofTemperatureCorrect->setValue(-5.3);
    Input_Brix->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
    Input_ApparentDensity->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
}

void SugarMeasure::Ini_Widget()
{
    Input_Brix = new QDoubleSpinBox;   Input_Brix->setRange(9, 25);
    Input_ApparentDensity = new QDoubleSpinBox; Input_ApparentDensity->setSingleStep(0.0001);   Input_ApparentDensity->setRange(1.0329, 1.1020);

    Output_TemperatureCorrect = new QDoubleSpinBox; Output_TemperatureCorrect->setSingleStep(0);

    Output_KLJDivisorofTemperatureCorrect = new QDoubleSpinBox; Output_KLJDivisorofTemperatureCorrect->setDecimals(2);
    Output_KLJDivisorofTemperatureCorrect->setRange(-100,100);

    Output_DirectPol = new QDoubleSpinBox; Output_DirectPol->setSingleStep(0.1);    Output_DirectPol->setRange(-100,100);

    Output_Transformpol = new QDoubleSpinBox; Output_Transformpol->setSingleStep(0.1);
    Output_Transformpol->setRange(0,100); Output_Transformpol->setPrefix("-");

    Output_g_Value = new QDoubleSpinBox; Output_Correct_Pol = new QDoubleSpinBox;
    Input_Sugar = new QDoubleSpinBox; Input_Sugar->setSuffix("%");

    Input_Brix->setAccelerated(true);    Input_ApparentDensity->setAccelerated(true);
    Output_DirectPol->setAccelerated(true);    Output_Transformpol->setAccelerated(true);
    Input_Brix->setDecimals(1);          Input_ApparentDensity->setDecimals(4);
    Output_DirectPol->setDecimals(1);          Output_Transformpol->setDecimals(1);

    Output_TemperatureCorrect->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));//隐藏上下箭头
    Output_KLJDivisorofTemperatureCorrect->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
    Output_g_Value->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
    Output_Correct_Pol->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
    Input_Sugar->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                   QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));

    Input_Temperature = new QSpinBox; Input_Temperature->setAccelerated(true); Input_Temperature->setRange(10,30);
    Input_ObservedBrix = new QSpinBox; Input_ObservedBrix->setAccelerated(true); Input_ObservedBrix->setRange(11,24);

    Input_KLJDivisorofTemperature = new QDoubleSpinBox; Input_KLJDivisorofTemperature->setAccelerated(true);
    Input_KLJDivisorofTemperature->setRange(0,35); Input_KLJDivisorofTemperature->setSingleStep(0.1);
     Input_KLJDivisorofTemperature->setDecimals(1);

    PB_Determine = new QPushButton("确定");
    PB_CalculateResult = new QPushButton("计算结果");
    PB_ClearData = new QPushButton("清除数据");
    setArrow = new QRadioButton("是否启用锤度与视密度快速相互查询");
}

void SugarMeasure::Ini_Layout()
{
    setFixedSize(350,407);; setWindowTitle("蔗糖分计算程序");  QPalette pt;
    pt.setBrush(QPalette::Background,QBrush(QPixmap(":/ui/海大.png"))); setPalette(pt);

    QVBoxLayout *pvl = new QVBoxLayout(this);
    QHBoxLayout *phl1 = new QHBoxLayout;
    QHBoxLayout *phl2 = new QHBoxLayout;
    QHBoxLayout *phl3 = new QHBoxLayout;
    QHBoxLayout *phl4 = new QHBoxLayout;//放置直接转光度和转化转光度
    QHBoxLayout *phl5 = new QHBoxLayout;//放置表外糖锤度和表外视密度
    QHBoxLayout *phl6 = new QHBoxLayout;//放置g值
    QHBoxLayout *phl7 = new QHBoxLayout;//放置二次旋光法校正转光度
    QHBoxLayout *phl8 = new QHBoxLayout;//蔗糖分
    QHBoxLayout *phl9 = new QHBoxLayout;//放置两个按钮

    QFormLayout *pfl1 = new QFormLayout; QFormLayout *pfl2 = new QFormLayout;
    QFormLayout *pfl3 = new QFormLayout; QFormLayout *pfl4 = new QFormLayout;
    QFormLayout *pfl5 = new QFormLayout; QFormLayout *pfl6 = new QFormLayout;
    QFormLayout *pfl7 = new QFormLayout; QFormLayout *pfl8 = new QFormLayout;
    QFormLayout *pfl9 = new QFormLayout; QFormLayout *pfl10 = new QFormLayout;
    QFormLayout *pfl11 = new QFormLayout; QFormLayout *pfl12 = new QFormLayout;

    pfl1->addRow(new QLabel("校正糖锤度:"),Input_Brix);
    pfl2->addRow(new QLabel("视密度值:"),Input_ApparentDensity);
    pfl3->addRow(new QLabel("蔗汁温度:"),Input_Temperature);
    pfl4->addRow(new QLabel("观测糖锤度:"),Input_ObservedBrix);
    pfl5->addRow(new QLabel("锤度校正数:"),Output_TemperatureCorrect);
    pfl6->addRow(new QLabel("半值直接转光度:"),Output_DirectPol);
    pfl7->addRow(new QLabel("半值转化转光度:"),Output_Transformpol);
    pfl8->addRow(new QLabel("测转化转光度的蔗汁温度:"),Input_KLJDivisorofTemperature);
    pfl9->addRow(new QLabel("对应更正数:"),Output_KLJDivisorofTemperatureCorrect);
    pfl10->addRow(new QLabel("g值:"),Output_g_Value);
    pfl11->addRow(new QLabel("二次旋光法校正转光度:"),Output_Correct_Pol);
    pfl12->addRow(new QLabel("蔗糖分:"),Input_Sugar);

    phl1->addLayout(pfl1);   phl1->addLayout(pfl2);
    phl2->addLayout(pfl3);   phl2->addLayout(pfl4);
    phl3->addWidget(setArrow);   phl3->addStretch(1);    phl3->addWidget(PB_Determine);
    phl4->addLayout(pfl6);   phl4->addLayout(pfl7);
    phl5->addLayout(pfl8);   phl5->addLayout(pfl9);
    phl6->addLayout(pfl10);  phl6->addStretch(1);   phl6->addLayout(pfl11);
    phl7->addStretch(3);  phl7->addWidget(PB_CalculateResult);  phl7->addStretch(1);
    phl7->addWidget(PB_ClearData);   phl7->addStretch(3);

    pvl->addLayout(phl2);   pvl->addLayout(pfl5);
    pvl->addStretch(1);     pvl->addLayout(phl1);
    pvl->addLayout(phl3);   pvl->addStretch(1);
    pvl->addLayout(phl4);   pvl->addStretch(1);
    pvl->addLayout(phl5);   pvl->addStretch(4);
    pvl->addLayout(phl6);   pvl->addLayout(pfl12); pvl->addLayout(phl7);


}

void SugarMeasure::Ini_Connect()
{
    Ini_InputConnect();
    Ini_OutputConnect();
    Ini_ButtonConnect();
}

void SugarMeasure::Ini_InputConnect()
{
    connect(Input_Brix,SIGNAL(valueChanged(double)),this,SLOT(GetApparentDensityforshow(double)));
    connect(Input_ApparentDensity,SIGNAL(valueChanged(double)),this,SLOT(GetBrixforshow(double)));
    connect(Input_Temperature,SIGNAL(valueChanged(int)),this,SLOT(SaveTemperature(int)));
    connect(Input_ObservedBrix,SIGNAL(valueChanged(int)),this,SLOT(SaveObservedBrix(int)));
    connect(Input_KLJDivisorofTemperature,SIGNAL(valueChanged(double)),this,SLOT(SaveTemperature_KLJ(double)));
    connect(Input_KLJDivisorofTemperature,SIGNAL(valueChanged(double)),this,SLOT(CalculateKLJ_CorrectValue()));
}

void SugarMeasure::Ini_OutputConnect()
{
    connect(Output_TemperatureCorrect,SIGNAL(valueChanged(double)),this,SLOT(SetCorrectBrix(double)));
    connect(Output_DirectPol,SIGNAL(valueChanged(double)),this,SLOT(SaveDirectPol(double)));
    connect(Output_Transformpol,SIGNAL(valueChanged(double)),this,SLOT(SaveTransformPol(double)));
}

void SugarMeasure::Ini_ButtonConnect()
{
    connect(PB_Determine,SIGNAL(clicked()),this,SLOT(GetBrixCorrection()));
    connect(PB_Determine,SIGNAL(clicked()),this,SLOT(SetCorrectBrix(double)));
    connect(PB_Determine,SIGNAL(clicked()),this,SLOT(SetbDeterminePress()));
    connect(PB_CalculateResult,SIGNAL(clicked()),this,SLOT(Calculate_g()));
    connect(PB_CalculateResult,SIGNAL(clicked()),this,SLOT(Calculate_S()));
    connect(PB_CalculateResult,SIGNAL(clicked()),this,SLOT(Finalresult()));
    connect(PB_ClearData,SIGNAL(clicked()),this,SLOT(ClearData()));
    connect(setArrow,SIGNAL(toggled(bool)),this,SLOT(ArrowShoworHide(bool)));
}

void SugarMeasure::GetBrixforshow(double AD)
{
    QMap<float,float>::iterator enditer = Brix_Transform_DtoB_Table->end();
    QMap<float,float>::iterator iter = Brix_Transform_DtoB_Table->find(AD);

    if(iter!=enditer){//查询到无效值时令Input_Brix的数值保持不变
        BeforeValue = *Brix_Transform_DtoB_Table->find(AD);//保存当前value
        Input_Brix->setValue(*Brix_Transform_DtoB_Table->find(AD));
    }
    else{
        Input_Brix->setValue(BeforeValue);//若key无效,则使用之前保存的value
    }
}

void SugarMeasure::GetApparentDensityforshow(double Brix)
{
    if(Input_Brix->value())
    Input_ApparentDensity->setValue(*Brix_Transform_BtoD_Table->find(Brix));
}

void SugarMeasure::GetBrixCorrection()
{
    if(!Input_Temperature->text().isEmpty()&&!Input_ObservedBrix->text().isEmpty()){//蔗汁温度和观测锤度任意值未输入,此函数都不响应
        bDeterminePress = true;//设置"确定"按钮按下标志为true;
    int T_vectorOffset = JuiceTemperature - 10;
    int B_vectorOffset = ObservedBrix - 11;
    if(JuiceTemperature<20){
       Output_TemperatureCorrect->setPrefix("-");
       Output_TemperatureCorrect->setValue((*Brix_Correct_Table)[T_vectorOffset][B_vectorOffset]);
    }
    else if(JuiceTemperature>20){
        T_vectorOffset--;//因为容器里不存在10°c这一行信息，所以事实上11°c占用了10°c的位置，所以要读取11°c的信息要把位置重定位到10°c的位置
        Output_TemperatureCorrect->setPrefix("+");
        Output_TemperatureCorrect->setValue((*Brix_Correct_Table)[T_vectorOffset][B_vectorOffset]);
    }
    else
        Output_TemperatureCorrect->setValue(0);
    }
}

void SugarMeasure::CalculateKLJ_CorrectValue()
{
    KLJ_CorrectValue = 0.53*(JuiceTemperature_KLJ-20);
    Output_KLJDivisorofTemperatureCorrect->setValue(KLJ_CorrectValue);
}

void SugarMeasure::Calculate_g()
{
    g_value = 0.5 * Input_Brix->value() * Input_ApparentDensity->value();
    Output_g_Value->setValue(g_value);
}

void SugarMeasure::Calculate_S()
{
    S_value = ((DirectPol*2 - TransformPol*2) / (132.56 - 0.0794*(13 - g_value) - KLJ_CorrectValue)) * 100;
    Output_Correct_Pol->setValue(S_value);
}

void SugarMeasure::Finalresult()
{
    float tmp1 = Output_g_Value->value();
    float tmp2 = Output_Correct_Pol->value();

    Output_g_Value->setValue(0);
    Output_Correct_Pol->setValue(0);
    Input_Sugar->setValue(0);

    QMessageBox ErrorMessage;
    ErrorMessage.setWindowTitle("错误断言");
    ErrorMessage.setIcon(QMessageBox::Critical);
    ErrorMessage.addButton(QMessageBox::Yes);
    ErrorMessage.setText("输入非法值或缺少依赖项!");

    if(COTTROL){//蔗汁温度不等于20°c,校正数不为0和蔗汁温度20°c,校正数为0时数值才有效
        if(Input_Sugar->value()==99.99){
            ErrorMessage.exec();
            return;
        }
        else if(Output_DirectPol->text().isEmpty()||Output_Transformpol->text().isEmpty()){
            ErrorMessage.exec();
            return;
        }
        else if(Input_KLJDivisorofTemperature->text().isEmpty()){
            ErrorMessage.exec();
            return;
        }
        if(bDeterminePress){//若未按下确定键,就不显示结果
        Input_Sugar->setValue((260.73-ObservedBrix) * S_value / 1000);
        Output_g_Value->setValue(tmp1);
        Output_Correct_Pol->setValue(tmp2);
        }
  }else{
        ErrorMessage.setText("请按下\"确定\"按钮更新锤度校正数之后再尝试计算结果!");
        ErrorMessage.exec();
        return;
    }

    if(!bDeterminePress){
       ErrorMessage.setText("请按下\"确定\"按钮更新锤度校正数之后再尝试计算结果!");
       ErrorMessage.exec();
       return;
    }
     bDeterminePress = false;//重置"确定"按钮的按下标志
}

void SugarMeasure::ClearData()
{
    Output_TemperatureCorrect->setPrefix(" ");

    Output_TemperatureCorrect->setValue(0);  Input_Sugar->setValue(0);
    Output_g_Value->setValue(0); Output_Correct_Pol->setValue(0);
    Output_KLJDivisorofTemperatureCorrect->setValue(0);

    Input_Temperature->clear(); Input_ObservedBrix->clear();
    Output_DirectPol->clear(); Output_Transformpol->clear();
    Input_KLJDivisorofTemperature->clear();
}

void SugarMeasure::ArrowShoworHide(bool bshowArrow)
{
    if(bshowArrow){
        Input_Brix->setStyleSheet(QString(""));
        Input_ApparentDensity->setStyleSheet(QString(""));
        QMessageBox ErrorMessage;
        ErrorMessage.setWindowTitle("注意事项");
        ErrorMessage.setText("       开启此选项将打开数值上下调整箭头,对于糖锤度而言,它的数值变化是连续的,每次通过上下箭头调整示数都有对应的视密度值。\n\
        而视线密度值变化是不连续的,所以若调整上下箭头调整示数不一定能得到对应的糖锤度,若不存在对应的糖锤度,则会显示为上一有效的视密度所对应的糖锤度,请务必注意这一点！\n\
        此功能只是额外用于糖锤度和视密度之间转换的,不使用此功能不影响最终结果,但不要在需要计算试验结果时使用此功能,否则将得到错误的实验结果!");
        ErrorMessage.addButton(QMessageBox::Yes);
        ErrorMessage.setIcon(QMessageBox::Warning);
        ErrorMessage.exec();
    }
    else{
        Input_Brix->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                       QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
        Input_ApparentDensity->setStyleSheet(QString("QDoubleSpinBox{border:1px solid #242424;}\
                                       QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"));
    }
}

void SugarMeasure::SetCorrectBrix(double Correctvalue)
{
    if(Input_Temperature->value()<20){
        QString Str = QString::number(Input_ObservedBrix->value()-Correctvalue,'.',2);
        if(Str[4]>='5'){
            Str.resize(4);
        Input_Brix->setValue(Str.toDouble()+0.1);
        }else{
            Str.resize(4);
            Input_Brix->setValue(Str.toDouble());
        }
    }
    else{
        QString Str = QString::number(Input_ObservedBrix->value()+Correctvalue,'.',2);
        if(Input_ObservedBrix->value()==25)//25+校正数会导致溢出
            Input_Brix->setValue(Input_ObservedBrix->value());
        if(Str[4]>='5'){
            Str.resize(4);
        Input_Brix->setValue(Str.toDouble()+0.1);
        }else{
            Str.resize(4);
            Input_Brix->setValue(Str.toDouble());
            }
    }
}

void SugarMeasure::Ini_Correct_Table()
{   //vector[20][15]
    Brix_Correct_Table = new QVector<QVector<float>>;
    for(int i=0; i<20; i++){//设置除了20°c以外的10-30°c对应锤度的容器
        Brix_Correct_Table->push_back(*new QVector<float>);
    }
    (*Brix_Correct_Table)[0]={0.44,   0.45, 0.46, 0.47, 0.48, 0.49, 0.50, 0.50, 0.51, 0.52, 0.53, 0.54, 0.55, 0.56, 0.57};
    (*Brix_Correct_Table)[1]={0.41,   0.42, 0.42, 0.43, 0.44, 0.45, 0.46, 0.46, 0.47, 0.48, 0.49, 0.49, 0.50, 0.50, 0.51};
    (*Brix_Correct_Table)[2]={0.37,	0.38, 0.38,	0.39, 0.40,	0.41, 0.41,	0.42, 0.42,	0.43, 0.44,	0.44, 0.45,	0.45, 0.46};
    (*Brix_Correct_Table)[3]={0.33,	0.33, 0.34,	0.34, 0.35,	0.36, 0.36,	0.37, 0.37,	0.38, 0.39,	0.39, 0.40,	0.40, 0.41};
    (*Brix_Correct_Table)[4]={0.29,	0.30, 0.30,	0.31, 0.31,	0.32, 0.32,	0.33, 0.33,	0.34, 0.34,	0.35, 0.35,	0.36, 0.36};
    (*Brix_Correct_Table)[5]={0.24,	0.25, 0.25,	0.26, 0.26,	0.26, 0.27,	0.27, 0.28,	0.28, 0.28,	0.29, 0.29,	0.30, 0.30};
    (*Brix_Correct_Table)[6]={0.20,	0.21, 0.21,	0.22, 0.22,	0.22, 0.22,	0.23, 0.23,	0.23, 0.23,	0.24, 0.24,	0.25, 0.25};
    (*Brix_Correct_Table)[7]={0.15,	0.16, 0.16,	0.16, 0.16,	0.16, 0.16,	0.17, 0.17,	0.18, 0.18,	0.18, 0.19,	0.19, 0.19};
    (*Brix_Correct_Table)[8]={0.10,	0.10, 0.11,	0.11, 0.11,	0.11, 0.11,	0.12, 0.12,	0.12, 0.12,	0.12, 0.13,	0.13, 0.13};
    (*Brix_Correct_Table)[9]={0.05,	0.05, 0.06,	0.06, 0.06,	0.06, 0.06,	0.06, 0.06,	0.06, 0.06,	0.06, 0.06,	0.06, 0.06};
    (*Brix_Correct_Table)[10]={0.06,  0.06,   0.06,	0.06,	0.06,	0.06,	0.06,	0.06,	0.06,	0.06,	0.06,	0.06,	0.07,	0.07,	0.07};
    (*Brix_Correct_Table)[11]={0.11,  0.11,   0.12,	0.12,	0.12,	0.12,	0.12,	0.12,	0.12,	0.12,	0.12,	0.12,	0.13,	0.13,	0.13};
    (*Brix_Correct_Table)[12]={0.17,  0.17,   0.17,	0.17,	0.17,	0.17,	0.18,	0.18,	0.19,	0.19,	0.19,	0.19,	0.20,	0.20,	0.20};
    (*Brix_Correct_Table)[13]={0.23,  0.23,   0.24,	0.24,	0.24,	0.24,	0.25,	0.25,	0.26,	0.26,	0.26,	0.26,	0.27,	0.27,	0.27};
    (*Brix_Correct_Table)[14]={0.30,  0.30,   0.31,	0.31,	0.31,	0.31,	0.31,	0.32,	0.32,	0.32,	0.32,	0.33,	0.33,	0.34,	0.34};
    (*Brix_Correct_Table)[15]={0.36,  0.36,   0.37,	0.37,	0.37,	0.38,	0.38,	0.39,	0.39,	0.40,	0.40,	0.40,	0.40,	0.40,	0.40};
    (*Brix_Correct_Table)[16]={0.42,  0.43,   0.43,	0.44,	0.44,	0.44,	0.45,	0.45,	0.46,	0.46,	0.46,	0.47,	0.47,	0.48,	0.47};
    (*Brix_Correct_Table)[17]={0.49,  0.50,   0.50,	0.51,	0.51,	0.52,	0.52,	0.53,	0.53,	0.54,	0.54,	0.55,	0.55,	0.56,	0.56};
    (*Brix_Correct_Table)[18]={0.57,  0.57,   0.58,	0.58,	0.59,	0.59,	0.60,	0.60,	0.61,	0.61,	0.61,	0.62,	0.62,	0.63,	0.63};
    (*Brix_Correct_Table)[19]={0.64,  0.64,   0.65,	0.65,	0.66,	0.66,	0.67,	0.67,	0.68,	0.68,	0.68,	0.69,	0.69,	0.70,	0.70};
}

void SugarMeasure::Ini_Brix_ApparentDensity_Table()
{
   Brix_Vector = new QVector<float>;   Brix_Vector->reserve(160);
   ApparentDensity_Vector = new QVector<float>;   ApparentDensity_Vector->reserve(160);
   Brix_Transform_BtoD_Table = new QMap<float,float>;
   Brix_Transform_DtoB_Table = new QMap<float,float>;
   *Brix_Vector ={
          9.0,	9.1,	9.2,	9.3,	9.4,	9.5,	9.6,	9.7,	9.8,	9.9,
          10.0,	10.1,	10.2,	10.3,	10.4,	10.5,	10.6,	10.7,	10.8,	10.9,
          11.0,	11.1,	11.2,	11.3,	11.4,	11.5,	11.6,	11.7,	11.8,	11.9,
          12.0,	12.1,	12.2,	12.3,	12.4,	12.5,	12.6,	12.7,	12.8,	12.9,
          13.0,	13.1,	13.2,	13.3,	13.4,	13.5,	13.6,	13.7,	13.8,	13.9,
          14.0,	14.1,	14.2,	14.3,	14.4,	14.5,	14.6,	14.7,	14.8,	14.9,
          15.0,	15.1,	15.2,	15.3,	15.4,	15.5,	15.6,	15.7,	15.8,	15.9,
          16.0,	16.1,	16.2,	16.3,	16.4,	16.5,	16.6,	16.7,	16.8,	16.9,
          17.0,	17.1,	17.2,	17.3,	17.4,	17.5,	17.6,	17.7,	17.8,	17.9,
          18.0,	18.1,	18.2,	18.3,	18.4,	18.5,	18.6,	18.7,	18.8,	18.9,
          19.0,	19.1,	19.2,	19.3,	19.4,	19.5,	19.6,	19.7,	19.8,	19.9,
          20.0,	20.1,	20.2,	20.3,	20.4,	20.5,	20.6,	20.7,	20.8,	20.9,
          21.0,	21.1,	21.2,	21.3,	21.4,	21.5,	21.6,	21.7,	21.8,	21.9,
          22.0,	22.1,	22.2,	22.3,	22.4,	22.5,	22.6,	22.7,	22.8,	22.9,
          23.0,	23.1,	23.2,	23.3,	23.4,	23.5,	23.6,	23.7,	23.8,	23.9,
          24.0,	24.1,	24.2,	24.3,	24.4,	24.5,	24.6,	24.7,	24.8,	24.9,
          };
   *ApparentDensity_Vector = {
       1.0329,	1.0333,	1.0337,	1.0342,	1.0346,	1.0350,	1.0354,	1.0358,	1.0362,	1.0366,
       1.0371,	1.0375,	1.0379,	1.0383,	1.0387,	1.0391,	1.0395,	1.0399,	1.0404,	1.0408,
       1.0412,	1.0416,	1.0420,	1.0424,	1.0429,	1.0433,	1.0437,	1.0441,	1.0445,	1.0449,
       1.0454,	1.0458,	1.0462,	1.0466,	1.0470,	1.0475,	1.0479,	1.0483,	1.0487,	1.0491,
       1.0496,	1.0500,	1.0504,	1.0508,	1.0513,	1.0517,	1.0521,	1.0525,	1.0530,	1.0534,
       1.0538,	1.0542,	1.0547,	1.0551,	1.0555,	1.0559,	1.0564,	1.0568,	1.0572,	1.0576,
       1.0581,	1.0585,	1.0589,	1.0594,	1.0598,	1.0606,	1.0606,	1.0611,	1.0615,	1.0619,
       1.0624,	1.0628,	1.0632,	1.0637,	1.0641,	1.0645,	1.0650,	1.0654,	1.0658,	1.0663,
       1.0667,	1.0671,	1.0676,	1.0680,	1.0684,	1.0689,	1.0693,	1.0697,	1.0702,	1.0706,
       1.0711,	1.0715,	1.0719,	1.0724,	1.0728,	1.0732,	1.0737,	1.0741,	1.0746,	1.0750,
       1.0754,	1.0759,	1.0763,	1.0768,	1.0772,	1.0776,	1.0781,	1.0785,	1.0790,	1.0794,
       1.0799,	1.0803,	1.0808,	1.0812,	1.0816,	1.0821,	1.0825,	1.0830,	1.0834,	1.0839,
       1.0843,	1.0848,	1.0852,	1.0857,	1.0861,	1.0866,	1.0870,	1.0875,	1.0879,	1.0884,
       1.0888,	1.0893,	1.0897,	1.0902,	1.0906,	1.0911,	1.0915,	1.0920,	1.0924,	1.0929,
       1.0933,	1.0938,	1.0942,	1.0947,	1.0951,	1.0956,	1.0961,	1.0965,	1.0970,	1.0974,
       1.0979,	1.0983,	1.0988,	1.0993,	1.0997,	1.1002,	1.1006,	1.1011,	1.1015,	1.1020,
            };
   for(int i=0; i<160; i++){
       Brix_Transform_BtoD_Table->insert((*Brix_Vector)[i],(*ApparentDensity_Vector)[i]);
   }
   for(int i=0; i<160; i++){
       Brix_Transform_DtoB_Table->insert((*ApparentDensity_Vector)[i],(*Brix_Vector)[i]);
   }
}


