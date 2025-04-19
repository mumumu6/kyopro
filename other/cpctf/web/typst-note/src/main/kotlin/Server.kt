package space.cpctf.typstnote

import io.ktor.http.*
import io.ktor.serialization.kotlinx.json.*
import io.ktor.server.application.*
import io.ktor.server.http.content.*
import io.ktor.server.jetty.jakarta.*
import io.ktor.server.plugins.contentnegotiation.*
import io.ktor.server.request.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import kotlinx.serialization.Serializable
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.nio.file.Files
import java.nio.file.Paths

@Serializable
data class PublishRequest(
    val pageId: String,
    val content: String,
)

val logger: Logger = LoggerFactory.getLogger("TypstNoteServer")

fun main(args: Array<String>) {
    logger.info("FLAG: ${System.getenv("FLAG")}")
    EngineMain.main(args)
}

fun Application.module() {
    install(ContentNegotiation) {
        json()
    }

    routing {
        staticResources("/", "static")

        post("/api/request-publish") {
            try {
                val request = call.receive<PublishRequest>()
                if (request.pageId.trim().isEmpty()) {
                    throw IllegalArgumentException("Page ID cannot be empty")
                }
                if (request.pageId.length > 25) {
                    throw IllegalArgumentException("Page ID is too long")
                }
                val pdfName = "\"${request.pageId.replace(Regex("""(?=["`\\])"""), "\\\\")}.pdf\""

                // Compile the Typst code into PDF
                Files.createDirectories(Paths.get("requests"))
                val compileProcess = ProcessBuilder("bash", "-c", "typst compile - requests/$pdfName").start()
                compileProcess.outputStream.use {
                    it.write(request.content.toByteArray())
                    it.flush()
                }
                if (compileProcess.waitFor() != 0) {
                    throw RuntimeException(compileProcess.errorStream.bufferedReader().readText())
                }

                logger.info("Received publish request: $pdfName")
                call.respondText("Publish request received", status = HttpStatusCode.OK)
            } catch (e: Exception) {
                // DO NOT respond with the error message for security reasons
                call.respondText("Error processing publish request", status = HttpStatusCode.BadRequest)
            }
        }
    }
}
