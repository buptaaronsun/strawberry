
<?php
	//$name=$_GET["name"];
	$hostname_conn = "";//服务器公网ip
	$database_conn = "";//自己建数据库名
	$username_conn = "";//数据库用户名
	$password_conn = "";//数据库密码
	//连接MYSQL数据库
	$conn = mysqli_connect($hostname_conn, $username_conn, $password_conn,$database_conn);
	if(!$conn){
		
		echo "连接不成功！";	
	}
	$sql = "SELECT *FROM week order by id desc limit 0,31;";
 	mysqli_query($conn, "set names 'utf8'");
	$result = mysqli_query($conn, $sql);
	class Week{
	//public $xxx;xxx为数据名称
		public $temp;
		public $hum;
		public $press;
		public $height;
		public $anglex;
		public $angley;
		public $anglez;
		public $ax;
		public $ay;
		public $az;
		public $wx;
		public $wy;
		public $wz;
		public $B;
		public $createtime;
	}
	$data = array();
	if (mysqli_num_rows($result) > 0) {
		while($row = mysqli_fetch_assoc($result)) {
			$week=new Week();
			$week->temp=$row["temp"];
			$week->hum=$row["hum"];
			$week->press=$row["press"];
			$week->height=$row["height"];
			$week->anglex=$row["anglex"];
			$week->angley=$row["angley"];
			$week->anglez=$row["anglez"];
			$week->ax=$row["ax"];
			$week->ay=$row["ay"];
			$week->az=$row["az"];
			$week->wx=$row["wx"];
			$week->wy=$row["wy"];
			$week->wz=$row["wz"];
			$week->B=$row["B"];
			$week->createtime=$row["createtime"];
			$data[] = $week;
			
			}
			echo json_encode($data,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);//将请求结果转换为json格式
 
			}
　　?>
