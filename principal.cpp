#include "principal.h"
#include "ui_principal.h"



#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    // CREACION DE PINCEL
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // ASIGNACION DE PINCEL AL PINTOR
    mPainter->setPen(pincel);
    // CONDICIONAL PARA SABER CUAL BOTON HA SIDO APLASTADO
    if(m_opcion == 1){
        // CONTROLA LA POSICION INICIAL EN DONDE SE HIZO CLICK
        mInicial = event->pos();
        // VARAIBLE QUE DA ACCESO AL DIBUJO LIBRE
        mPuedeDibujar = true;
    }else if(m_opcion == 2){
        // CONDICIONAL PARA COMPROBAR SI ES PRIMER O SEGUNDO PUNTO DIBUJADO
        if(m_compbdr){
            // CONTROLA LA POSICION INICIAL EN DONDE SE HIZO CLICK
            mInicial = event->pos();
            // MUESTRA MENSAJE DE QUE SE CREO EL PRIMER PUNTO
            ui->statusbar->showMessage("Primer punto, elija el segundo",1500);
        }else{
            // CONTROLA LA POSICION FINAL EN DONDE SE HIZO CLICK
            mFinal = event->pos();
            // SE CREA LA LINEA
            mPainter->drawLine(mInicial, mFinal);
            // MUESTRA MENSAJE DE QUE SE CREO EL OBJETO
            ui->statusbar->showMessage("¡LINEA CREADA!",1000);
            update();
        }
    }else if(m_opcion == 3){
            if (m_compbdr){
                // CONTROLA LA POSICION INICIAL EN DONDE SE HIZO CLICK
                mInicial = event->pos();
                // MUESTRA MENSAJE DE QUE SE CREO EL PRIMER PUNTO
                ui->statusbar->showMessage("Primer punto, elija el punto final del rectangulo",1500);
            }else {
                // CONTROLA LA POSICION FINAL EN DONDE SE HIZO CLICK
                mFinal = event->pos();
                // SE CREA EL RECTANGULO
                mPainter->drawRect(mInicial.x(), mInicial.y(), mFinal.x()-mInicial.x(), mFinal.y()-mInicial.y());
                // MUESTRA MENSAJE DE QUE SE CREO EL OBJETO
                ui->statusbar->showMessage("¡RECTANGULO CREADO!",1500);
                update();
            }
    }else if (m_opcion == 4){
        if(m_compbdr){
            // CONTROLA LA POSICION INICIAL EN DONDE SE HIZO CLICK
            mInicial = event->pos();
            // MUESTRA MENSAJE DE QUE SE CREO EL PRIMER PUNTO
            ui->statusbar->showMessage("Primer punto, elija el radio",1500);
        }else{
            // CONTROLA LA POSICION FINAL EN DONDE SE HIZO CLICK
            mFinal = event->pos();
            // SE CREA LA CIRCUNFERENCIA
            mPainter->drawEllipse(mInicial,mFinal.rx()-mInicial.rx(),mFinal.ry()-mInicial.ry());
            // MUESTRA MENSAJE DE QUE SE CREO EL OBJETO
            ui->statusbar->showMessage("¡CIRCUNFERENCIA CREADA!",1500);
            update();
        }
    }
    // SE RESTABLECE LA VARIABLE PARA VOLVERLA A USAR
    m_compbdr= !m_compbdr;
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        event->accept();
        return;
    }
    // Capturar el punto donde se suelta el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    // Mostrar el número de líneas en la barra de estado
    ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
    // Actualizar la interfaz
    update();
    // actualizar el punto inicial
    mInicial = mFinal;
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    mPuedeDibujar = false;
    // Aceptar el vento
    event->accept();

}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 100);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes (*.png)");
    if ( !nombreArchivo.isEmpty() ){
        if (mImagen->save(nombreArchivo))
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        else
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
    }
}



void Principal::on_actionLibre_triggered()
{
    m_opcion = 1;
}


void Principal::on_actionLineas_triggered()
{
    m_opcion = 2;
    m_compbdr = true;
}


void Principal::on_actionRect_nculos_triggered()
{
    m_opcion = 3;
    m_compbdr = true;
}


void Principal::on_actionCircunferencias_triggered()
{
    m_opcion = 4;
    m_compbdr = true;
}

