var http=require('http');
var url=require('url');
var fs=require('fs');

var mime = {
   'html' : 'text/html',
   'css'  : 'text/css',
   'js'   : 'application/javascript',
   'png'  : 'image/png',
   'dae'  : 'model/vnd.collada+xml'
};

var server=http.createServer(function(request,response){
    var objeturl = url.parse(request.url);
    var path='public'+objeturl.pathname;
    if (path=='public/')
        path='public/index.html';
    fs.exists(path,function(exists){
        if (exists) {
            fs.readFile(path,function(error,content){
                if (error) {
                    response.writeHead(500, {'Content-Type': 'text/plain'});
                    response.write('Internal error: '+ error);
                    response.end();
                } else {

                    var vec = path.split('.');
                    var ext=vec[vec.length-1];
                    var mimearchive=mime[ext];
                    response.writeHead(200,{'Content-Type': mimearchive});

                    response.write(content);
                    response.end();
                }
            });
        } else {
            response.writeHead(404, {'Content-Type': 'text/html'});
            response.write('<!doctype html><html><head></head><body>404 Not Found</body></html>');
            response.end();
        }
    });
});

var port = 7777;
server.listen(port);

console.log('Runnig TurtlebotViewer on http://localhost:'+port);
