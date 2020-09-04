import com.klaxit.hiddensecrets.Utils
import io.kotlintest.shouldBe
import io.kotlintest.specs.WordSpec

class UtilsTest : WordSpec({

    "Using getUnderScoredPackageName()" should {
        "transform package separator" {
            val sourcePackage = "com.klaxit.test"
            Utils.getUnderScoredPackageName(sourcePackage) shouldBe "com_klaxit_test"
        }
    }

    "Using encodeSecret()" should {
        "encode String with a seed" {
            val key = "keyToEncode"
            val packageName = "com.klaxit.test"
            Utils.encodeSecret(key,packageName) shouldBe "{ 0x5b, 0x6, 0x18, 0x31, 0xb, 0x72, 0x57, 0x5, 0x5d, 0x57, 0x3 }"
        }
    }
})