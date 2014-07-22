/** HTTP client class.
 * https://github.com/azadkuh/qhttp
 *
 * @author amir zamani
 * @version 2.0.0
 * @date 2014-07-11
  */

#ifndef QHTTPCLIENT_HPP
#define QHTTPCLIENT_HPP

///////////////////////////////////////////////////////////////////////////////
#include "qhttpfwd.hpp"

#include <QTcpSocket>
#include <QUrl>
///////////////////////////////////////////////////////////////////////////////
namespace qhttp {
namespace client {
///////////////////////////////////////////////////////////////////////////////

/** a simple and async HTTP client class which sends a request to an HTTP server and parses the
 *  corresponding response.
 * This class internally handles the memory management and life cycle of QHttpRequest and
 *  QHttpResponse instances. you do not have to manually delete or keep their pointers.
 * in fact the QHttpRequest and QHttpResponse object will be deleted when the internal socket
 *  disconnects.
 */
class QHttpClient : public QTcpSocket
{
    Q_OBJECT

    Q_PROPERTY(quint32 timeOut READ timeOut WRITE setTimeOut)

public:
    explicit    QHttpClient(QObject *parent = nullptr);

    virtual    ~QHttpClient();

    /** tries to connect to a HTTP server.
     *  when the connection is made, creates and emits a QHttpRequest instance
     *   by @sa httpConnected(QHttpRequest*)
     * @param method an HTTP method, ex: GET, POST, ...
     * @param url specifies server's address, port and optional path and query strings.
     * @return true if the url is valid or false (no connection will be made).
     */
    bool        request(THttpMethod method, QUrl url);

    /** checks if the connetion to the server is open. */
    bool        isOpen() const;


    /** returns time-out value [mSec] for open connections (sockets).
     *  @sa setTimeOut(). */
    quint32     timeOut()const;

    /** set time-out for new open connections in miliseconds [mSec].
     * each connection will be forcefully closed after this timeout.
     *  a zero (0) value disables timer for new connections. */
    void        setTimeOut(quint32);

signals:
    /** emitted when a new HTTP connection to the server is established.
     * if you overload onRequestReady this signal won't be emitted.
     * @sa onRequestReady
     * @sa QHttpRequest
     */
    void        httpConnected(QHttpRequest* req);

    /** emitted when a new response is received from the server.
     * if you overload onResponseReady this signal won't be emitted.
     * @sa onResponseReady
     * @sa QHttpResponse
     */
    void        newResponse(QHttpResponse* res);


protected:
    /** called when a new HTTP connection is established.
     * you can overload this method, the default implementaion only emits connected().
     * @param req use this request instance for assinging the
     *   request headers and sending optional body.
     * @see httpConnected(QHttpRequest*)
     */
    virtual void onRequestReady(QHttpRequest* req);

    /** called when a new response is received from the server.
     *  you can overload this method, the default implementaion only emits newResponse().
     * @param res use this instance for reading incoming response.
     * @see newResponse(QHttpResponse*)
     */
    virtual void onResponseReady(QHttpResponse* res);

protected:
    explicit    QHttpClient(QHttpClientPrivate&, QObject*);

    void        timerEvent(QTimerEvent*);

    Q_DECLARE_PRIVATE(QHttpClient)
    Q_DISABLE_COPY(QHttpClient)
    QScopedPointer<QHttpClientPrivate>  d_ptr;
};

///////////////////////////////////////////////////////////////////////////////
} // namespace client
} // namespace qhttp
///////////////////////////////////////////////////////////////////////////////
#endif // define QHTTPCLIENT_HPP