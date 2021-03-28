#ifndef TCP_STATS_H
#define TCP_STATS_H

/*Global variables to calculate all traffic and processed connections
of whole application.
*/

#ifndef WIN32
extern	m_var<long long> tcp_total_send;
extern 	m_var<long long> tcp_total_recv;
extern	m_var<long long> tcp_total_accepted_connections;
#else
extern 	m_var<__int64> tcp_total_send;
extern	m_var<__int64> tcp_total_recv;
extern	m_var<__int64> tcp_total_accepted_connections;
#endif

#endif
