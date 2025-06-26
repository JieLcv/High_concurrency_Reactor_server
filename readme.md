# 基于linux的多反应堆服务器

使用c++实现了一个轻量级的网络服务器，支持多个浏览器对服务器资源的访问，通过线程池提高 并发度，支持select，poll和epoll多种io多路复用机制。

1. 利用多态将select, poll和epoll封装为不同类型的监听器
2. 线程池中主线程负责监听客户端的连接，将监听到的连接添加到其他线程的任务队列中，多个子线程 处理各自任务队列中的任务并监听浏览器的请求。
3. 对于浏览器的每个请求，都会创建一个tcpConnection对象负责解析请求和发送请求的资源。

项目在vs2019上实现，关于项目内容的详细介绍请参考 [reactor服务器]:https://blog.csdn.net/m0_46168092/article/details/146928330?sharetype=blogdetail&sharerId=146928330&sharerefer=PC&sharesource=m0_46168092&sharefrom=mp_from_link

