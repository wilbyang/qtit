#include <QApplication>
#include <QtWebSockets/QWebSocket>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QCandlestickSet>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr)
    {
        initializeUI();
        initializeWebSocket();
    }
    ~MainWindow()
    {

    }

private slots:
    void onConnected()
    {
        qDebug() << "WebSocket connected";
        // Subscribe to specific stock data
        m_webSocket.sendTextMessage("{\"action\": \"subscribe\", \"symbol\": \"AAPL\"}");
    }
    void onDisconnected()
    {
        qDebug() << "WebSocket disconnected";
    }
    void updateChart(double open, double high, double low, double close, const QString &timestamp)
    {
        // Create new candlestick set
        QCandlestickSet *set = new QCandlestickSet(open, high, low, close, QDateTime::fromString(timestamp, Qt::ISODate).toMSecsSinceEpoch());

        // Add to series and store in vector
        m_series->append(set);
        m_sets.append(set);

        // Keep only last 30 candlesticks
        if (m_sets.size() > 30)
        {
            m_series->remove(m_sets.first());
            delete m_sets.first();
            m_sets.removeFirst();
        }

        // Update axes
        m_chart->axes(Qt::Horizontal).first()->setRange(QDateTime::fromMSecsSinceEpoch(m_sets.first()->timestamp()), QDateTime::fromMSecsSinceEpoch(m_sets.last()->timestamp()));

        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::min();
        for (const auto &set : m_sets)
        {
            minY = std::min(minY, set->low());
            maxY = std::max(maxY, set->high());
        }

        m_chart->axes(Qt::Vertical).first()->setRange(minY * 0.99, maxY * 1.01);
    }
    void onMessageReceived(const QString &message)
    {
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        if (!doc.isObject())
            return;

        QJsonObject obj = doc.object();

        // Parse OHLC data
        double open = obj["open"].toDouble();
        double high = obj["high"].toDouble();
        double low = obj["low"].toDouble();
        double close = obj["close"].toDouble();
        QString timestamp = obj["timestamp"].toString();

        updateChart(open, high, low, close, timestamp);
    }

private:
    void initializeUI()
    {
        m_series = new QCandlestickSeries();
        m_series->setIncreasingColor(QColor(Qt::green));
        m_series->setDecreasingColor(QColor(Qt::red));

        m_chart = new QChart();
        m_chart->addSeries(m_series);
        m_chart->setTitle("Stock OHLC Data");
        m_chart->createDefaultAxes();
        m_chart->legend()->hide();

        // Create chart view
        m_chartView = new QChartView(m_chart);
        m_chartView->setRenderHint(QPainter::Antialiasing);

        // Set up main window
        setCentralWidget(m_chartView);
        resize(800, 600);
    }
    void initializeWebSocket()
    {
        const QUrl url(QStringLiteral("ws://your-websocket-server.com/stocks"));

        connect(&m_webSocket, &QWebSocket::connected, this, &MainWindow::onConnected);
        connect(&m_webSocket, &QWebSocket::disconnected, this, &MainWindow::onDisconnected);
        connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::onMessageReceived);

        m_webSocket.open(url);
    }
    

    QWebSocket m_webSocket;
    QCandlestickSeries *m_series;
    QChart *m_chart;
    QChartView *m_chartView;
    QVector<QCandlestickSet *> m_sets;
};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#include "qt_websocket_stock.moc"