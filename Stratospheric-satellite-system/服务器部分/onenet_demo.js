//Setup basic express server
var express = require('express');
var app = express();
var server = require('http').createServer(app);

var bodyParser = require('body-parser');


var crypto = require('crypto');

var port = process.env.PORT || 3000;
var mysql      = require('mysql');
var connection = mysql.createConnection({     
    host     : '',      //服务器公网IP 
    user     : '',              //数据库用户名
    password : '',       //数据库密码
    port: '3306',                   
    database: '', //数据库名称
  }); 
var temps = 0;
var hum = 0;
var presss = 0;
var heights = 0;
var anglexs= 0;
var angleys = 0;
var anglezs =0;
var ax = 0;
var ay = 0;
var az = 0;
var wx =0;
var wy = 0;
var wz = 0;
var Bx = 0;
var Byy = 0;
var Bz = 0;
var i;
 var userAddSql = 'insert into week (temp,hum,press,height,anglex,angley,anglez,ax,ay,az,wx,wy,wz,Bx,Bz,Byy) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)';
connection.connect(function(err) {
    if(err) console.log('fail');
    else{
	console.log('success');
	
}})

//*********以下内容与推送无关************//
server.listen(port, function () {
    console.log('Server listening at port %d', port);
});

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));

// Routing
app.use(express.static(__dirname + '/www'));
//************以上内容与推送无关*********//

///////////////////////////////////////////////////////////////////////////////////


app.get('/dataPort', function (req, res) {

    console.log(req.query);
    //res.body = req.query;

    res.send(req.query.msg);
});


 
app.post('/dataPort', function (req, res) {
 
	 res.send({code: 200});
     console.log(req.body); 
	 
//	示例：if(req.body.msg.ds_id=='xxx（onenet上数据的名称）'){  
//      temps=req.body.msg.value;
 //      }
 if(req.body.msg.ds_id=='temp'){  
      temps=req.body.msg.value;
       }

    if(req.body.msg.ds_id=='press'){  
        presss=req.body.msg.value;
       }
   if(req.body.msg.ds_id=='hum'){  
         heights=req.body.msg.value;
      } 
   if(req.body.msg.ds_id=='anglex'){  
        anglexx=req.body.msg.value;
       }
    if(req.body.msg.ds_id=='angley'){  
        angleyx=req.body.msg.value;
       }
    if(req.body.msg.ds_id=='anglez'){  
        anglezx=req.body.msg.value;
		var param = [temps,'0',presss,heights,anglexs,angleys,anglezs,'0','0','0','0','0','0','0','0','0'];
connection.query(userAddSql,param,function(err,rs){
  if(err){
    console.log('insert err:',err.message);
    return;
  }
console.log('insert success');})
       }
	 
	
	
})



