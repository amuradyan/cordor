package cordor

import cordor.util.Haversine
import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.types.{ArrayType, DoubleType, IntegerType, LongType, StringType, StructType}
import org.apache.spark.sql.functions._
import org.apache.spark.sql.{Encoders}

case class Junction(lat: Float, lon: Float)

case class RouteSegment(id: String)

case class RawData(beacon_id: String,
                    beacon_lat: Double,
                    beacon_lon: Double,
                    route_segment_id: String,
                    timestamp: Long,
                    transport_id: String,
                    number_of_vehicles: Integer,
                    lat_1: Double,
                    lon_1: Double,
                    lat_2: Double,
                    lon_2: Double
                  ) {
  def this() = this("", 0D, 0D, "", 0L, "", 0, 0D, 0D, 0D, 0D)
}

object Cordor {
  val rawDataSchema =  new StructType()
    .add("beacon_id", StringType)
    .add("beacon_lat", DoubleType)
    .add("beacon_lon", DoubleType)
    .add("route_segment_id", StringType)
    .add("timestamp", LongType)
    .add("transport_id", StringType)
    .add("number_of_vehicles", IntegerType)
    .add("lat_1", DoubleType)
    .add("lon_1", DoubleType)
    .add("lat_2", DoubleType)
    .add("lon_2", DoubleType)

  def distanceFrom(A: (Double, Double), B: (Double, Double), C: (Double, Double)) = {
    // Sides of the triangle
    val AB = Haversine(A, B)
    val BC = Haversine(B, C)
    val AC = Haversine(A, C)

    val distance = if(AB + AC == BC) { // The point is on the line
      0D
    } else {
      // Herons formula
      val s = (AB + BC + AC) / 2
      val area = math.sqrt(s * (s - AB) * (s - BC) * (s - AC))
      (2 * area) / BC
    }

    distance
  }

  implicit val rawDataEncoder = Encoders.bean(classOf[RawData])

  def main(args: Array[String]): Unit = {
    val spark = SparkSession
      .builder()
      .master("local[*]")
      .appName("cordor")
      .getOrCreate()

    val inputDirectory = "hdfs://localhost:9000/cordor/input"
    val rawData = spark
        .read
        .schema(rawDataSchema)
        .json(inputDirectory)
        .as[RawData]

    // Filter out all beacon readings that are off a route segment by a threshold
    val beaconsOnTheRoad = rawData.filter(ping => {
      val beaconPosition = (ping.beacon_lon, ping.beacon_lat)
      val routeSegmentEnd = (ping.lon_1, ping.lat_1)
      val routeSegmentStart = (ping.lon_2, ping.lat_2)
      val distance = distanceFrom(beaconPosition, routeSegmentEnd, routeSegmentStart)
      println("*******************************************************************????")
      println(distance)
      println("*******************************************************************????")
      distance < 10 // meters
    })

    val output = beaconsOnTheRoad.groupBy("route_segment_id", "timestamp").agg(sum("number_of_vehicles"))
    output.coalesce(1).write.json("hdfs://localhost:9000/cordor/output")
  }
}
