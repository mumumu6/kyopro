val kotlin_version: String by project
val ktor_version: String by project
val slf4j_version: String by project

plugins {
    kotlin("jvm") version "2.1.20"
    id("org.jetbrains.kotlin.plugin.serialization") version "2.1.20"
    id("io.ktor.plugin") version "3.1.2"
}

group = "space.cpctf.typstnote"
version = "1.0.0"

application {
    mainClass = "io.ktor.server.jetty.jakarta.EngineMain"

    val isDevelopment: Boolean = project.ext.has("development")
    applicationDefaultJvmArgs = listOf(
        "-Dio.ktor.development=$isDevelopment", "-Dorg.slf4j.simpleLogger.logFile=System.out"
    )
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("org.slf4j:slf4j-simple:$slf4j_version")
    implementation("io.ktor:ktor-server-core-jvm:$ktor_version")
    implementation("io.ktor:ktor-server-jetty-jakarta:$ktor_version")
    implementation("io.ktor:ktor-server-config-yaml:$ktor_version")
    implementation("io.ktor:ktor-server-content-negotiation:$ktor_version")
    implementation("io.ktor:ktor-serialization-kotlinx-json:$ktor_version")
}
