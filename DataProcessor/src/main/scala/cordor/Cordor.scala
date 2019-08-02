package cordor

import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.types.{ArrayType, FloatType, IntegerType, LongType, StringType, StructType}
import org.apache.spark.sql.functions._

case class Junction(lat: Float, lon: Float)

case class RouteSegment(id: String)

case class RawData(
                    beacon_id: String,
                    beacon_lat: Float,
                    beacon_lon: Float,
                    route_segment_id: String,
                    timestamp: Long,
                    transport_id: String,
                    number_of_vehicles: Integer,
                    lat_1: Float,
                    lon_1: Float,
                    lat_2: Float,
                    lon_2: Float
                  )

object Cordor {
  def main(args: Array[String]): Unit = {

    val rawDataSchema =  new StructType()
            .add("beacon_id", StringType)
            .add("route_segment_id", StringType)
            .add("timestamp", LongType)
            .add("transport_id", StringType)
            .add("number_of_vehicles", IntegerType)
            .add("lat_1", FloatType)
            .add("lon_1", FloatType)
            .add("lat_2", FloatType)
            .add("lon_1", FloatType)
        )

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

    val output = rawData.groupBy("route_segment_id", "timestamp").agg(sum("number_of_vehicles"))
    output.coalesce(1).write.json("hdfs://localhost:9000/cordor/output/output.json")
  }
}
